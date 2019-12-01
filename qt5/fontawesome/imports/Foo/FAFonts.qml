pragma Singleton
import QtQuick 2.12

QtObject {
id: fonts
readonly property FontLoader fontAwesomeRegular: FontLoader {
    source: "./fa-regular-400.ttf"
}
readonly property FontLoader fontAwesomeLight: FontLoader {
    source: "./fa-light-300.ttf"
}
readonly property FontLoader fontAwesomeSolid: FontLoader {
    source: "./fa-solid-900.ttf"
}

readonly property string regularIcons: fontAwesomeRegular.name
readonly property string lightIcons: fontAwesomeLight.name
readonly property string solidIcons: fontAwesomeSolid.name

}