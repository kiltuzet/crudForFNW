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
    property var logLines: []        // каждая строка — отдельный элемент массива
    property int pageSize: 50
    property int currentPage: 0
    property int logVersion: 0       // триггер перерисовки

    // Видимый текст текущей страницы
    property string visibleText: getVisibleText()

    function getVisibleText() {
        const start = currentPage * pageSize
        const end   = (currentPage + 1) * pageSize
        return logLines.slice(start, end).join("\n")
    }

    // Добавить одну строку
    function appendLine(line) {
        logLines.push(line)
        logVersion++
    }

    // Добавить многострочный блок как отдельные строки
    function appendBlock(text) {
        const lines = String(text).split(/\r?\n/)
        for (var i = 0; i < lines.length; ++i) {
            appendLine(lines[i])
        }
        // Автопереход на последнюю страницу при новых записях (можно отключить)
        currentPage = Math.max(0, Math.ceil(logLines.length / pageSize) - 1)
    }

    // Перерисовка при изменениях
    onCurrentPageChanged: visibleText = getVisibleText()
    onPageSizeChanged:    visibleText = getVisibleText()
    onLogVersionChanged:  visibleText = getVisibleText()

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
                        appendLine(ok ? "[INIT] База подключена" : "[INIT] Ошибка подключения")
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
                        appendLine("[PROCESS] Пустой JSON")
                        return
                    }
                    nutritionConverter.processJsonResponse(jsonInput.text, testUserId)
                }
            }

            Button {
                text: "Показать сегодняшнюю статистику"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const stats = nutritionConverter.getConsumedToday(testUserId, "")
                    appendLine("=== DAILY STATS ===")
                    console.log(stats);
                    appendBlock(JSON.stringify(stats, null, 4))
                }
            }

            Button {
                text: "Показать продукты"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const p = nutritionConverter.getAllProducts()
                    appendLine("=== PRODUCTS LIST ===")
                    appendBlock(JSON.stringify(p, null, 4))
                }
            }

            Button {
                text: "Показать эмоции"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const e = nutritionConverter.getAllEmotions()
                    appendLine("=== EMOTIONS LIST ===")
                    appendBlock(JSON.stringify(e, null, 4))
                }
            }

            Button {
                text: "Показать упражнения"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const ex = nutritionConverter.getExerciseEntriesByUser(testUserId, "")
                    appendLine("=== EXERCISES LIST ===")
                    appendBlock(JSON.stringify(ex, null, 4))
                }
            }

            Button {
                text: "Показать пользователей"
                enabled: nutritionConverter.isConnected
                onClicked: {
                    const u = nutritionConverter.getAllUsers()
                    appendLine("=== USERS LIST ===")
                    appendBlock(JSON.stringify(u, null, 4))
                }
            }

            Button {
                text: "Очистить лог"
                enabled: true
                onClicked: {
                    logLines = []
                    logVersion++
                    currentPage = 0
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
                    text: visibleText
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
        function onDataProcessed(success, message) { appendLine("[RESULT] " + message) }
        function onErrorOccurred(msg) { appendLine("[ERROR] " + msg) }
    }
}
