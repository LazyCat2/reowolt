#ifndef STYLE_H
#define STYLE_H

#define STYLE_ACCENT "rgb(253, 102, 113)"
#define STYLE_ACCENT_DARK "rgb(200, 50, 60)"

#define STYLE_CSS                                                              \
R"(
QPushButton.accent {
    background: )" STYLE_ACCENT R"(;
    color: white;
}

QPushButton, QLineEdit {
    padding: 10px;
    border-radius: 5px;
}

QPushButton {
    border: none;
    outline: none;
}

QPushButton.primary:hover {
    background-color: )" STYLE_ACCENT_DARK R"(;
}

QPushButton.secondary:hover,
.server-icon {
    background-color: #15000000;
}

QPushButton.small-padding {
    padding: 3px !important;
}

/* QPushButton:focus {  /* Looks ugly */
    border-color: )" STYLE_ACCENT_DARK R"(;
    border-width: 2px;
    border-style: solid;
} */

QLineEdit:focus {
    border-color: )" STYLE_ACCENT R"(;
    border-width: 2px;
    padding: 8px;
    border-style: solid;
}

.logo {
    color: )" STYLE_ACCENT R"(;
}

.transparent {
    opacity: 0.7;
}

.transparent:hover {
    opacity: 1;
}

.servericon {
    padding: 10px;
    margin: 10px;
}
)"

#endif // STYLE_H
