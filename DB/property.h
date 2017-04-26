#pragma once

#define PROPERTY_GS(type, name, var) Q_PROPERTY(type name READ name WRITE set##name) private: type var; public: inline type name() { return var; } public: inline void set##name(type name) { var = name; }

#define PROPERTY_G(type, name, var) Q_PROPERTY(type name READ name) private: type var; public: inline type name() { return var; }
