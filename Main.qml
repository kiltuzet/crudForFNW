import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: 900
    height: 600
    visible: true
    title: "Nutrition JSON → SQL Tester"

    property int testUserId: 1
    property string testDate: "2025-01-01"

    // Лог + пагинация
    property var logLines: []
    property int pageSize: 50
    property int currentPage: 0
    property int logVersion: 0
    property string visibleText: getVisibleText()

    function getVisibleText() {
        const start = currentPage * pageSize
        const end   = (currentPage + 1) * pageSize
        return logLines.slice(start, end).join("\n")
    }

    function appendLine(line) {
        logLines.push(line)
        logVersion++
    }

    function appendBlock(text) {
        const lines = String(text).split(/\r?\n/)
        for (var i = 0; i < lines.length; ++i) {
            appendLine(lines[i])
        }
        currentPage = Math.max(0, Math.ceil(logLines.length / pageSize) - 1)
    }

    onCurrentPageChanged: visibleText = getVisibleText()
    onPageSizeChanged:    visibleText = getVisibleText()
    onLogVersionChanged:  visibleText = getVisibleText()

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        GroupBox {
            title: "Ввод JSON"
            Layout.fillWidth: true
            Layout.preferredHeight: 250

            TextArea {
                id: jsonInput
                textFormat: TextEdit.PlainText
                wrapMode: TextArea.Wrap
                placeholderText: "{ \"userId\": 1, \"date\": \"2025-01-01\", \"consumed\": [...], \"emotions\": [...] }"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Отправить JSON"
                onClicked: {
                    if (jsonInput.text.length === 0) {
                        appendLine("[PROCESS] Пустой JSON")
                        return
                    }
                    let data
                    try {
                        data = JSON.parse(jsonInput.text)
                    } catch (e) {
                        appendLine("[ERROR] Invalid JSON: " + e)
                        return
                    }

                    // EMOTIONS
                    if (data.emotions) {
                        for (let e of data.emotions) {
                            EmotionsService.addEmotion(e.name, e.classification)
                        }
                        appendLine("[PROCESS] Emotions added")
                    }

                    // CONSUMED
                    if (data.consumed) {
                        for (let c of data.consumed) {
                            ConsumedService.addConsumedEntry(
                               data
                            )
                        }
                        appendLine("[PROCESS] Consumed entries added")
                    }

                    // EXERCISES
                    if (data.exercises) {
                        for (let ex of data.exercises) {
                            ExerciseService.addExerciseEntry(
                                data.userId, data.date,
                                ex.exerciseName, ex.duration,
                                ex.caloriesBurned, ex.timestamp
                            )
                        }
                        appendLine("[PROCESS] Exercises added")
                    }

                    // DAILY STATS
                    const stats = DailyStatisticsService.getDailyStatistics(data.userId, data.date)
                    appendLine("=== DAILY STATS ===")
                    appendBlock(JSON.stringify(stats, null, 4))
                }
            }

            Button {
                text: "Очистить лог"
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
}
