CREATE TABLE host (
    id     INTEGER PRIMARY KEY AUTOINCREMENT
                   NOT NULL,
    secret TEXT,
    name   TEXT,
    status BOOLEAN DEFAULT (0)
);

CREATE TABLE usb (
    id     INTEGER PRIMARY KEY AUTOINCREMENT
                   NOT NULL,
    VID    TEXT,
    PID    TEXT,
    serial TEXT,
    name   TEXT
);

CREATE TABLE rules (
    id      INTEGER PRIMARY KEY AUTOINCREMENT
                    NOT NULL,
    usb_id  INTEGER REFERENCES usb (id) ON DELETE CASCADE
                                        ON UPDATE CASCADE,
    host_id INTEGER REFERENCES host (id) ON DELETE CASCADE
                                         ON UPDATE CASCADE,
    value  BOOLEAN DEFAULT (0)
);
