#include <usbinfo.h>
#include <QRegularExpression>
#include <QSettings>

#include <windows.h>
#include <string>
using std::wstring;
#include <QException>

static QString FindDeviceKeyInMountedDevices(QChar letter)
{
    QSettings reader(R"(HKEY_LOCAL_MACHINE\SYSTEM\MountedDevices)", QSettings::NativeFormat);
    QString key;
    foreach(QString item, reader.allKeys())
    {
        if (item.contains(QString("%1:").arg(letter.toUpper())))
        {
            key = item;
        }
    }
    key.replace("/", "\\");
    return key;
}

static QString GetDeviceStringForKey(QString key)
{
    LONG lResult;
    HKEY hKey;
    lResult = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"SYSTEM\\MountedDevices", 0, KEY_READ, &hKey);
    DWORD dType;
    DWORD dbData = 512;
    BYTE *data = new BYTE[512];
    ZeroMemory(data, 512);
    lResult = RegQueryValueExA(hKey, key.toStdString().c_str(), 0, &dType, data, &dbData);
    RegCloseKey(hKey);
    QString deviceString = QString::fromWCharArray(reinterpret_cast<wchar_t*>(data));
    return deviceString;
}

static QString ParceDeviceString(QString DeviceString)
{
    QRegularExpression re(R"(\_\?\?\_USBSTOR#[\w\d\&\_\.]+#([A-Z0-9]+).+)");
    QRegularExpressionMatch result = re.match(DeviceString);
    if (result.hasMatch())
    {
        return result.captured(1);
    }
    else
        throw std::invalid_argument("device string can't matching");
}

static QString GetSerialNumber(QChar letter)
{
    QString key = FindDeviceKeyInMountedDevices(letter);
    QString deviceString = GetDeviceStringForKey(key);
    return ParceDeviceString(deviceString);
}

static QString GetIDString(QString serial)
{
    QSettings reader(R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB)", QSettings::NativeFormat);
    foreach (auto item, reader.allKeys())
    {
        if (item.contains(serial))
        {
            return item;
        }
    }
}

static QString GetDeviceName(QString serial)
{
    QSettings reader(R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USBSTOR)", QSettings::NativeFormat);
    foreach (auto item, reader.allKeys())
    {
        if (item.contains(serial) && item.contains("FriendlyName"))
        {
            return reader.value(item).value<QString>();
        }
    }
}

static UsbInfo ParceIDString(QString idString)
{
    QRegularExpression re("VID_([A-Fa-f0-9]{4})&PID_([A-Fa-f0-9]{4})");
    auto result = re.match(idString);
    if (result.hasMatch())
    {
        UsbInfo info;
        info.VID = result.captured(1);
        info.PID = result.captured(2);
        return info;
    }
    else
        throw std::invalid_argument("ID string can't matching");
}

UsbInfo GetDriveInfo(QChar letter)
{
    QString serial = GetSerialNumber(letter);
    QString IDString = GetIDString(serial);
    UsbInfo info = ParceIDString(IDString);
    info.serial = serial;
    info.name = GetDeviceName(serial);
    return info;
}
