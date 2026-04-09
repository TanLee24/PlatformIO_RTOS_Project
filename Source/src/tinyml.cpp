#include "tinyml.h"

// Globals, for the convenience of one-shot setup.
namespace
{
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *input = nullptr;
    TfLiteTensor *output = nullptr;
    constexpr int kTensorArenaSize = 4 * 1024; // Adjust size based on your model
    uint8_t tensor_arena[kTensorArenaSize];
} // namespace

void setupTinyML()
{
    Serial.println("TensorFlow Lite Init....");
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(dht_anomaly_model_tflite); // g_model_data is from model_data.h
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Model provided is schema version %d, not equal to supported version %d.",
                               model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        error_reporter->Report("AllocateTensors() failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("TensorFlow Lite Micro initialized on ESP32.");
}

void tinyML(void *pvParameters)
{
    setupTinyML();

    while (1)
    {
        // Prepare input data (e.g., sensor readings)
        float t = glob_temperature;
        float h = glob_humidity;

        input->data.f[0] = t;
        input->data.f[1] = h;

        // Run inference
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk)
        {
            error_reporter->Report("Invoke failed");
            vTaskDelay(pdMS_TO_TICKS(5000));
            return;
        }

        // Get and process output
        float result = output->data.f[0];
        Serial.println("\n===== TINYML PREDICTION =====");
        Serial.printf("Input: Temp = %.1f°C | Hum = %.1f%%\n", t, h);

        if (result >= 0.5) {
            // Close to 1.0 means Label 1 (Abnormal)
            Serial.printf("AI OUTPUT: Abnormal (Confidence: %.1f%%)\n", result * 100);
            glob_ml_state = 1;
        } else {
            // Close to 0.0 means Label 0 (Normal)
            Serial.printf("AI OUTPUT: Normal (Confidence: %.1f%%)\n", (1.0 - result) * 100);
            glob_ml_state = 0;
        }
        Serial.println("================================");

        vTaskDelay(5000);
    }
}