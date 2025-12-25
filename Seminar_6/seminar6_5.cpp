#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;


struct ValidationRule {
    string fieldName;
    double minValue;
    double maxValue;
    string unit;

    ValidationRule(const string& name = "", double min = 0.0, double max = 0.0,
                   const string& unit = "")
        : fieldName(name), minValue(min), maxValue(max), unit(unit) {}
};
struct ValidationResult {
    string fieldName;
    double value;
    bool isValid;
    string message;

    ValidationResult(const string& name = "", double val = 0.0,
                     bool valid = false, const string& msg = "")
        : fieldName(name), value(val), isValid(valid), message(msg) {}
};

class DataValidator {
private:
    vector<ValidationRule> validationRules;
    vector<ValidationResult> validationResults;
    string reportFilename;
    int totalValidations;
    int successfulValidations;

    bool checkValue(const string& fieldName, double value) {
        for (const auto& rule : validationRules) {
            if (rule.fieldName == fieldName) {
                return (value >= rule.minValue && value <= rule.maxValue);
            }
        }
        return true;
    }

    string generateErrorMessage(const string& fieldName, double value,
                               const ValidationRule& rule) {
        ostringstream oss;
        oss << fieldName << ": ОШИБКА - ";

        if (value < rule.minValue) {
            oss << "значение " << value << rule.unit
                << " меньше минимального " << rule.minValue << rule.unit;
        } else if (value > rule.maxValue) {
            oss << "значение " << value << rule.unit
                << " превышает максимум " << rule.maxValue << rule.unit;
        }

        return oss.str();
    }

    string generateSuccessMessage(const string& fieldName, double value,
                                 const ValidationRule& rule) {
        ostringstream oss;
        oss << fieldName << ": УСПЕХ - значение " << value << rule.unit
            << " в допустимом диапазоне [" << rule.minValue
            << rule.unit << ", " << rule.maxValue << rule.unit << "]";
        return oss.str();
    }

public:
    DataValidator(const string& filename = "validation_report.txt")
        : reportFilename(filename), totalValidations(0), successfulValidations(0) {
        addDefaultValidationRules();
    }

    void addValidationRule(const string& field, double min, double max,
                          const string& unit = "") {
        validationRules.emplace_back(field, min, max, unit);
        cout << "Добавлено правило валидации: " << field
             << " [" << min << unit << ", " << max << unit << "]" << endl;
    }

    void addDefaultValidationRules() {
        addValidationRule("x", -10000.0, 10000.0, "м");
        addValidationRule("y", -10000.0, 10000.0, "м");
        addValidationRule("z", 0.0, 5000.0, "м");
        addValidationRule("speed", 0.0, 300.0, "м/с");
        addValidationRule("acceleration", -50.0, 20.0, "м/с²");
        addValidationRule("temperature", -50.0, 50.0, "°C");
        addValidationRule("pressure", 900.0, 1100.0, "гПа");
        addValidationRule("fuel", 0.0, 100.0, "%");
    }

    bool validateCoordinates(double x, double y, double z) {
        bool xValid = checkValue("x", x);
        bool yValid = checkValue("y", y);
        bool zValid = checkValue("z", z);
        bool allValid = xValid && yValid && zValid;

        ValidationRule xRule, yRule, zRule;
        for (const auto& rule : validationRules) {
            if (rule.fieldName == "x") xRule = rule;
            if (rule.fieldName == "y") yRule = rule;
            if (rule.fieldName == "z") zRule = rule;
        }
        if (!xRule.fieldName.empty()) {
            string msg = xValid ?
                generateSuccessMessage("Координата X", x, xRule) :
                generateErrorMessage("Координата X", x, xRule);
            validationResults.emplace_back("Координата X", x, xValid, msg);
        }

        if (!yRule.fieldName.empty()) {
            string msg = yValid ?
                generateSuccessMessage("Координата Y", y, yRule) :
                generateErrorMessage("Координата Y", y, yRule);
            validationResults.emplace_back("Координата Y", y, yValid, msg);
        }

        if (!zRule.fieldName.empty()) {
            string msg = zValid ?
                generateSuccessMessage("Координата Z", z, zRule) :
                generateErrorMessage("Координата Z", z, zRule);
            validationResults.emplace_back("Координата Z", z, zValid, msg);
        }

        string overallMsg;
        if (allValid) {
            overallMsg = "Координаты: УСПЕХ - все координаты в допустимых диапазонах";
        } else {
            overallMsg = "Координаты: ОШИБКА - некоторые координаты вне допустимых диапазонов";
        }
        validationResults.emplace_back("Координаты (общий)", 0, allValid, overallMsg);

        totalValidations += 3; // x, y, z
        if (allValid) successfulValidations += 3;

        return allValid;
    }

    bool validateSpeed(double speed) {
        bool isValid = checkValue("speed", speed);

        ValidationRule speedRule;
        for (const auto& rule : validationRules) {
            if (rule.fieldName == "speed") {
                speedRule = rule;
                break;
            }
        }

        if (!speedRule.fieldName.empty()) {
            string msg = isValid ?
                generateSuccessMessage("Скорость", speed, speedRule) :
                generateErrorMessage("Скорость", speed, speedRule);
            validationResults.emplace_back("Скорость", speed, isValid, msg);
        }

        totalValidations++;
        if (isValid) successfulValidations++;

        return isValid;
    }
    bool validateAcceleration(double acceleration) {
        bool isValid = checkValue("acceleration", acceleration);
        ValidationRule accelRule;
        for (const auto& rule : validationRules) {
            if (rule.fieldName == "acceleration") {
                accelRule = rule;
                break;
            }
        }
        if (!accelRule.fieldName.empty()) {
            string msg = isValid ?
                generateSuccessMessage("Ускорение", acceleration, accelRule) :
                generateErrorMessage("Ускорение", acceleration, accelRule);
            validationResults.emplace_back("Ускорение", acceleration, isValid, msg);
        }

        totalValidations++;
        if (isValid) successfulValidations++;

        return isValid;
    }
    bool validateField(const string& fieldName, double value) {
        bool isValid = checkValue(fieldName, value);
        ValidationRule fieldRule;
        for (const auto& rule : validationRules) {
            if (rule.fieldName == fieldName) {
                fieldRule = rule;
                break;
            }
        }
        if (!fieldRule.fieldName.empty()) {
            string msg = isValid ?
                generateSuccessMessage(fieldName, value, fieldRule) :
                generateErrorMessage(fieldName, value, fieldRule);
            string displayName = fieldName;
            transform(displayName.begin(), displayName.end(), displayName.begin(), ::toupper);
            validationResults.emplace_back(displayName, value, isValid, msg);
        }

        totalValidations++;
        if (isValid) successfulValidations++;

        return isValid;
    }

    // Генерация отчета валидации
    void generateValidationReport() {
        ofstream fout(reportFilename);
        if (!fout.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << reportFilename << endl;
            return;
        }

        fout << "=== ОТЧЕТ ВАЛИДАЦИИ ДАННЫХ ===" << endl;
        fout << "Дата: " << __DATE__ << " Время: " << __TIME__ << endl;
        fout << "==============================" << endl << endl;

        fout << "Результаты валидации:" << endl;
        fout << "---------------------" << endl;

        int errorCount = 0;
        int successCount = 0;

        for (const auto& result : validationResults) {
            fout << result.message << endl;
            if (!result.isValid) {
                errorCount++;
            } else {
                successCount++;
            }
        }

        fout << endl << "Статистика валидации:" << endl;
        fout << "---------------------" << endl;
        fout << "Всего проверок: " << totalValidations << endl;
        fout << "Успешных: " << successfulValidations << endl;
        fout << "Ошибок: " << (totalValidations - successfulValidations) << endl;

        double score = getValidationScore();
        fout << "Общий результат: " << fixed << setprecision(1)
             << score << "% данных валидны" << endl;

        fout << endl << "Подробная информация:" << endl;
        fout << "---------------------" << endl;
        for (const auto& rule : validationRules) {
            fout << rule.fieldName << ": [" << rule.minValue
                 << rule.unit << ", " << rule.maxValue << rule.unit << "]" << endl;
        }

        fout.close();

        cout << "Отчет валидации сохранен в файл: " << reportFilename << endl;

        // Выводим краткий отчет в консоль
        printSummaryToConsole();
    }

    // Вывод краткой сводки в консоль
    void printSummaryToConsole() {
        cout << "\n=== КРАТКАЯ СВОДКА ВАЛИДАЦИИ ===" << endl;

        // Выводим только ошибки
        bool hasErrors = false;
        for (const auto& result : validationResults) {
            if (!result.isValid && result.fieldName != "Координаты (общий)") {
                cout << result.message << endl;
                hasErrors = true;
            }
        }

        if (!hasErrors) {
            cout << "Все данные валидны!" << endl;
        }

        cout << "\nОбщий результат: " << fixed << setprecision(1)
             << getValidationScore() << "% данных валидны" << endl;
    }

    // Получение оценки валидации (в процентах)
    double getValidationScore() {
        if (totalValidations == 0) return 100.0;
        return (static_cast<double>(successfulValidations) / totalValidations) * 100.0;
    }

    // Очистка результатов
    void clearResults() {
        validationResults.clear();
        totalValidations = 0;
        successfulValidations = 0;
        cout << "Результаты валидации очищены." << endl;
    }

    // Показать все правила валидации
    void showValidationRules() {
        cout << "\n=== ПРАВИЛА ВАЛИДАЦИИ ===" << endl;
        for (const auto& rule : validationRules) {
            cout << rule.fieldName << ": [" << rule.minValue
                 << rule.unit << ", " << rule.maxValue << rule.unit << "]" << endl;
        }
    }

    // Получение списка результатов
    const vector<ValidationResult>& getResults() const {
        return validationResults;
    }

    // Проверка, есть ли ошибки
    bool hasErrors() const {
        return successfulValidations < totalValidations;
    }
};

// Функция для демонстрации работы валидатора
void demonstrateDataValidator() {
    cout << "=== СИСТЕМА ВАЛИДАЦИИ ДАННЫХ ===" << endl;

    // Создаем валидатор
    DataValidator validator("validation_report.txt");

    // Показываем правила валидации
    validator.showValidationRules();

    // Тестируем данные из задания
    cout << "\n=== ТЕСТИРОВАНИЕ ДАННЫХ ИЗ ЗАДАНИЯ ===" << endl;

    // Координаты из задания (невалидные)
    cout << "\n1. Валидация координат:" << endl;
    cout << "   x=5000.5, y=3000.2, z=10000.0" << endl;
    validator.validateCoordinates(5000.5, 3000.2, 10000.0);

    // Скорость из задания (невалидная)
    cout << "\n2. Валидация скорости:" << endl;
    cout << "   speed=350.0" << endl;
    validator.validateSpeed(350.0);

    // Ускорение из задания (невалидное)
    cout << "\n3. Валидация ускорения:" << endl;
    cout << "   acceleration=25.0" << endl;
    validator.validateAcceleration(25.0);

    // Генерируем отчет
    validator.generateValidationReport();

    // Тестируем валидные данные
    cout << "\n\n=== ТЕСТИРОВАНИЕ ВАЛИДНЫХ ДАННЫХ ===" << endl;
    validator.clearResults();

    // Валидные координаты
    cout << "\n1. Валидация координат:" << endl;
    cout << "   x=1000.0, y=2000.0, z=3000.0" << endl;
    validator.validateCoordinates(1000.0, 2000.0, 3000.0);

    // Валидная скорость
    cout << "\n2. Валидация скорости:" << endl;
    cout << "   speed=250.0" << endl;
    validator.validateSpeed(250.0);

    // Валидное ускорение
    cout << "\n3. Валидация ускорения:" << endl;
    cout << "   acceleration=15.0" << endl;
    validator.validateAcceleration(15.0);

    // Валидация дополнительных полей
    cout << "\n4. Валидация дополнительных параметров:" << endl;
    validator.validateField("temperature", 20.5);
    validator.validateField("pressure", 1013.25);
    validator.validateField("fuel", 75.5);

    // Генерируем отчет для валидных данных
    validator.generateValidationReport();

    // Тестирование смешанных данных
    cout << "\n\n=== ТЕСТИРОВАНИЕ СМЕШАННЫХ ДАННЫХ ===" << endl;
    validator.clearResults();

    // Смешанные данные (часть валидных, часть невалидных)
    cout << "\nСмешанные данные:" << endl;

    // Частично валидные координаты
    validator.validateCoordinates(-5000.0, 8000.0, 6000.0); // z невалидно

    // Валидная и невалидная скорость
    validator.validateSpeed(280.0);  // валидно
    validator.validateSpeed(-10.0);  // невалидно (отрицательная)

    // Валидное и невалидное ускорение
    validator.validateAcceleration(18.0);  // валидно
    validator.validateAcceleration(30.0);  // невалидно

    // Дополнительные поля
    validator.validateField("temperature", -60.0);  // невалидно
    validator.validateField("fuel", 110.0);         // невалидно (> 100%)

    // Генерируем финальный отчет
    validator.generateValidationReport();

    // Показать итоговую оценку
    cout << "\n=== ИТОГОВАЯ ОЦЕНКА ===" << endl;
    cout << "Процент валидных данных: " << fixed << setprecision(1)
         << validator.getValidationScore() << "%" << endl;

    if (validator.hasErrors()) {
        cout << "Обнаружены ошибки валидации!" << endl;
    } else {
        cout << "Все данные валидны!" << endl;
    }
}

int main() {
    demonstrateDataValidator();
    return 0;
}
