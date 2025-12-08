import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import com.nutrition.converter 1.0

ApplicationWindow {
    width: 900
    height: 600
    visible: true
    title: "Nutrition JSON → SQL Tester"

    property string databasePath: "nutrition_app.db"
    property int testUserId: 1

    // Лог + пагинация
    property var logLines: []
    property int pageSize: 50
    property int currentPage: 0
    property int logVersion: 0  // инкремент при каждом добавлении/очистке

    // Видимый текст страницы (реактивно зависит от версии, страницы и размера)
    property string visibleText: getVisibleText()

    function getVisibleText() {
        const start = currentPage * pageSize
        const end   = (currentPage + 1) * pageSize
        return logLines.slice(start, end).join("\n")
    }

    function appendLog(line) {
        logLines.push(line)
        logVersion++            // триггерим пересчёт
        visibleText = getVisibleText()
    }

    function redrawPage() {
        visibleText = getVisibleText()
    }

    // Пересчёт при изменении параметров пагинации
    onCurrentPageChanged: redrawPage()
    onPageSizeChanged: redrawPage()
    onLogVersionChanged: redrawPage()

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        GroupBox {
            title: "Подключение к базе данных"
            Layout.fillWidth: true

            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                TextField {
                    id: dbPathField
                    Layout.fillWidth: true
                    text: databasePath
                    placeholderText: "Путь к базе..."
                }

                Button {
                    text: "Инициализировать"
                    onClicked: {
                        const ok = nutritionConverter.initDatabase(dbPathField.text, ":/bdext.sql")
                        appendLog(ok ? "[INIT] База подключена" : "[INIT] Ошибка подключения")
                    }
                }

                Label { text: nutritionConverter.isConnected ? "Подключено" : "Нет подключения" }
            }
        }

        GroupBox {
            title: "Ввод JSON"
            Layout.fillWidth: true
            Layout.preferredHeight: 250

            TextArea {
                id: jsonInput
                textFormat: TextEdit.PlainText
                wrapMode: TextArea.Wrap
                placeholderText: "{ \"consumed\": [...], \"emotions\": [...] }"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Отправить JSON"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    if (jsonInput.text.length === 0) {
                        appendLog("[PROCESS] Пустой JSON")
                        return
                    }
                    nutritionConverter.processJsonResponse(jsonInput.text, testUserId)
                }
            }

            Button {
                text: "Показать сегодняшнюю статистику"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const stats = nutritionConverter.getDailyStatistics(testUserId, "")
                    appendLog("=== DAILY STATS ===")
                    appendLog(JSON.stringify(stats, null, 4))
                }
            }

            Button {
                text: "Показать продукты"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const p = nutritionConverter.getAllProducts()
                    appendLog("=== PRODUCTS LIST ===")
                    appendLog(JSON.stringify(p, null, 4))
                }
            }

            Button {
                text: "Показать эмоции"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const e = nutritionConverter.getAllEmotions()
                    appendLog("=== EMOTIONS LIST ===")
                    appendLog(JSON.stringify(e, null, 4))
                }
            }

            Button {
                text: "Показать упражнения"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const ex = nutritionConverter.getExerciseEntriesByUser(testUserId, "")
                    appendLog("=== EXERCISES LIST ===")
                    appendLog(JSON.stringify(ex, null, 4))
                }
            }

            Button {
                text: "Показать пользователей"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const u = nutritionConverter.getAllUsers()
                    appendLog("=== USERS LIST ===")
                    appendLog(JSON.stringify(u, null, 4))
                }
            }
        }

        GroupBox {
            title: "Лог выполнения"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 6

                TextArea {
                    id: logArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    readOnly: true
                    wrapMode: TextArea.Wrap
                    text: visibleText  // ключевая привязка
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Button {
                        text: "← Назад"
                        enabled: currentPage > 0
                        onClicked: currentPage--
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Страница " + (currentPage + 1) + " из " + Math.max(1, Math.ceil(logLines.length / pageSize))
                    }

                    Button {
                        text: "Вперёд →"
                        enabled: (currentPage + 1) * pageSize < logLines.length
                        onClicked: currentPage++
                    }
                }
            }
        }
    }

    Connections {
        target: nutritionConverter
        function onDataProcessed(success, message) { appendLog("[RESULT] " + message) }
        function onErrorOccurred(msg) { appendLog("[ERROR] " + msg) }
    }
}
