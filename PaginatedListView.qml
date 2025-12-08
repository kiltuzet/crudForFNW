import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: root
    property int pageSize: 10
    property int currentPage: 0
    property var fullModel: []
    property Component delegateComponent

    // Вырезка видимой страницы
    property var visibleModel: fullModel.slice(currentPage * pageSize,
                                               (currentPage + 1) * pageSize)

    ListView {
        id: listView
        Layout.fillWidth: true
        Layout.fillHeight: true
        model: visibleModel
        delegate: delegateComponent
        clip: true
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Button {
            text: "← Назад"
            enabled: root.currentPage > 0
            onClicked: {
                root.currentPage--
                root.visibleModel = root.fullModel.slice(root.currentPage * root.pageSize,
                                                         (root.currentPage + 1) * root.pageSize)
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Страница " + (root.currentPage + 1) + " из " + Math.ceil(root.fullModel.length / root.pageSize)
        }

        Button {
            text: "Вперёд →"
            enabled: (root.currentPage + 1) * root.pageSize < root.fullModel.length
            onClicked: {
                root.currentPage++
                root.visibleModel = root.fullModel.slice(root.currentPage * root.pageSize,
                                                         (root.currentPage + 1) * root.pageSize)
            }
        }
    }
}
