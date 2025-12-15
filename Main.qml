import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "JSON → Services → SQLite"

    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        padding: 8

        Label {
            text: "Input JSON"
            font.pixelSize: 16
        }

        TextArea {
            id: inputJson
            Layout.fillWidth: true
            Layout.fillHeight: true
            wrapMode: TextArea.Wrap
            font.family: "monospace"
            text: `{
  "userId": 1,
  "date": "2025-01-01",
  "emotions": [
    { "name": "positive", "classification": "POSITIVE" }
  ],
  "consumed": [
    {
      "productName": "гречка",
      "quantity": 200,
      "unit": "g",
      "proteins": 6,
      "fats": 2,
      "carbs": 40,
      "calories": 180,
      "timestamp": "08:30"
    }
  ],
  "exercises": [
    {
      "exerciseName": "running",
      "duration": 30,
      "caloriesBurned": 250,
      "timestamp": "18:00"
    }
  ]
}`
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "Process JSON"
                onClicked: processJson()
            }

            Label {
                id: statusLabel
                text: ""
                Layout.fillWidth: true
                color: "red"
            }
        }

        Label {
            text: "Result (Daily statistics)"
            font.pixelSize: 16
        }

        TextArea {
            id: outputJson
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            font.family: "monospace"
        }
    }

    function processJson() {
        statusLabel.text = ""

        let data
        try {
            data = JSON.parse(inputJson.text)
        } catch (e) {
            statusLabel.text = "Invalid JSON: " + e
            return
        }

        try {
            // 🔹 EMOTIONS
            if (data.emotions) {
                for (let e of data.emotions) {
                    EmotionsService.addEmotion(
                        e.name,
                        e.classification
                    )
                }
            }

            // 🔹 CONSUMED
            if (data.consumed) {
                for (let c of data.consumed) {
                    ConsumedService.addConsumedEntry(
                        data.userId,
                        data.date,
                        c.productName,
                        c.quantity,
                        c.unit,
                        c.proteins,
                        c.fats,
                        c.carbs,
                        c.calories,
                        c.timestamp
                    )
                }
            }

            // 🔹 EXERCISES
            if (data.exercises) {
                for (let ex of data.exercises) {
                    ExerciseService.addExerciseEntry(
                        data.userId,
                        data.date,
                        ex.exerciseName,
                        ex.duration,
                        ex.caloriesBurned,
                        ex.timestamp
                    )
                }
            }

            // 🔹 LOAD RESULT
            const stats = DailyStatisticsService.getDailyStatistics(
                data.userId,
                data.date
            )

            outputJson.text = JSON.stringify(stats, null, 2)
            statusLabel.text = "OK"
            statusLabel.color = "green"

        } catch (e) {
            statusLabel.text = "Runtime error: " + e
            statusLabel.color = "red"
        }
    }
}
