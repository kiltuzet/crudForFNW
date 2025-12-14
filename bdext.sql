-- Создание базы данных nutrition_app.db
-- SQLite database schema and initial data

-- 1. Таблица достижений
CREATE TABLE IF NOT EXISTS achievements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    unlocked BOOLEAN NOT NULL DEFAULT 0,
    description TEXT
);

-- 2. Таблица продуктов
CREATE TABLE IF NOT EXISTS products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL
);

-- 3. Таблица рецептов
CREATE TABLE IF NOT EXISTS recipes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL
);

-- 4. Таблица ингредиентов рецептов убрать все поля связанные с БЖУ
CREATE TABLE IF NOT EXISTS recipe_ingredients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    recipe_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL,
    FOREIGN KEY (recipe_id) REFERENCES recipes(id),
    FOREIGN KEY (product_id) REFERENCES products(id)
);

-- 5. Таблица пользователей
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    age INTEGER NOT NULL,
    gender TEXT NOT NULL CHECK (gender IN ('Мужской', 'Женский')),
    height REAL NOT NULL,
    weight REAL NOT NULL,
    goal TEXT NOT NULL CHECK (goal IN ('Сбросить вес', 'Сохранить вес', 'Набрать вес')),
    activity_level INTEGER NOT NULL CHECK (activity_level BETWEEN 1 AND 4)
);

-- 6. Таблица эмоций
CREATE TABLE IF NOT EXISTS emotions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    classification TEXT NOT NULL
);

-- 7. Таблица упражнений
CREATE TABLE IF NOT EXISTS exercises (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL
);

-- 8. Статистика по дням
CREATE TABLE IF NOT EXISTS daily_statistics (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    water_ml REAL NOT NULL DEFAULT 0,
    total_proteins REAL NOT NULL DEFAULT 0,
    total_fats REAL NOT NULL DEFAULT 0,
    total_carbs REAL NOT NULL DEFAULT 0,
    total_calories REAL NOT NULL DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES users(id),
    UNIQUE(user_id, date)
);

-- 9. Записи эмоций
CREATE TABLE IF NOT EXISTS emotion_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    emotion_id INTEGER NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (emotion_id) REFERENCES emotions(id)
);

-- 10. Записи потребленной еды
CREATE TABLE IF NOT EXISTS consumed_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    product_name TEXT NOT NULL,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL,
    calories REAL NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- 11. Записи упражнений
CREATE TABLE IF NOT EXISTS exercise_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    exercise_name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

---------------------------------------------------------------------
-- ДАННЫЕ ------------------------------------------------------------
---------------------------------------------------------------------

-- Достижения
INSERT INTO achievements (name, unlocked, description) VALUES
('Первый день', 0, 'Поздравляем! Вы используете приложение первый день.'),
('Первая неделя', 0, 'Отлично! Вы пользуетесь приложением уже неделю.'),
('Первый месяц', 0, 'Вы успешно использовали приложение в течение месяца.'),
('Мастер калорий', 0, 'Вы достигли своей дневной нормы калорий.'),
('Гид по воде', 0, 'Вы выпили рекомендованное количество воды за день.'),
('Эксперт рецептов', 0, 'Вы создали или использовали рецепты для приготовления блюд.'),
('Фитнес чемпион', 0, 'Вы сожгли более 1000 ккал за один день.');

-- Продукты
INSERT INTO products (name, quantity, unit, proteins, fats, carbs) VALUES
('Яблоко', 100, 'г', 0, 0, 14),
('Банан', 100, 'г', 1, 0, 23),
('Куриная грудка', 100, 'г', 31, 4, 0),
('Рис', 100, 'г', 3, 0, 28),
('Брокколи', 100, 'г', 3, 0, 7),
('Морковь', 100, 'г', 1, 0, 10),
('Картофель', 100, 'г', 2, 0, 17),
('Творог', 100, 'г', 14, 4, 3),
('Гречка', 100, 'г', 3, 1, 20),
('Овсянка', 100, 'г', 12, 7, 66);

-- Рецепты
INSERT INTO recipes (name, quantity, unit, proteins, fats, carbs) VALUES
('Куриный суп с овощами', 100, 'порция', 50, 6, 25),
('Овсяная каша с фруктами', 100, 'порция', 11, 6, 89),
('Гречневая каша с творогом', 100, 'порция', 10, 3, 28),
('Бананово-яблочный смузи', 100, 'стакан', 1, 0, 37),
('Салат из брокколи с творогом', 100, 'порция', 18, 5, 19);

---------------------------------------------------------------------
-- ИНГРЕДИЕНТЫ ДЛЯ РЕЦЕПТОВ -----------------------------------------
---------------------------------------------------------------------

-- Куриный суп (recipe_id = 1)
INSERT INTO recipe_ingredients
(recipe_id, product_id, name, quantity, unit, proteins, fats, carbs) VALUES
(1, 3, 'Куриная грудка', 150, 'г', 47, 5, 0),
(1, 7, 'Картофель', 100, 'г', 2, 0, 17),
(1, 6, 'Морковь', 50, 'г', 0, 0, 5),
(1, 5, 'Брокколи', 50, 'г', 1, 0, 4);

-- Овсяная каша (recipe_id = 2)
INSERT INTO recipe_ingredients
(recipe_id, product_id, name, quantity, unit, proteins, fats, carbs) VALUES
(2, 10, 'Овсянка', 80, 'г', 9, 5, 53),
(2, 1, 'Яблоко', 100, 'г', 0, 0, 14),
(2, 2, 'Банан', 50, 'г', 1, 0, 22);

-- Гречка с творогом (recipe_id = 3)
INSERT INTO recipe_ingredients
(recipe_id, product_id, name, quantity, unit, proteins, fats, carbs) VALUES
(3, 9, 'Гречка', 100, 'г', 3, 1, 20),
(3, 8, 'Творог', 50, 'г', 7, 2, 2),
(3, 1, 'Яблоко', 50, 'г', 0, 0, 7);

-- Смузи (recipe_id = 4)
INSERT INTO recipe_ingredients
(recipe_id, product_id, name, quantity, unit, proteins, fats, carbs) VALUES
(4, 2, 'Банан', 100, 'г', 1, 0, 23),
(4, 1, 'Яблоко', 100, 'г', 0, 0, 14);

-- Салат (recipe_id = 5)
INSERT INTO recipe_ingredients
(recipe_id, product_id, name, quantity, unit, proteins, fats, carbs) VALUES
(5, 5, 'Брокколи', 150, 'г', 4, 1, 11),
(5, 8, 'Творог', 100, 'г', 14, 4, 3),
(5, 6, 'Морковь', 50, 'г', 1, 0, 4);

---------------------------------------------------------------------
-- Пользователь ------------------------------------------------------
---------------------------------------------------------------------

INSERT INTO users (name, age, gender, height, weight, goal, activity_level)
VALUES ('User', 30, 'Мужской', 180, 75, 'Сохранить вес', 2);

---------------------------------------------------------------------
-- Индексы -----------------------------------------------------------
---------------------------------------------------------------------

CREATE INDEX IF NOT EXISTS idx_daily_statistics_user_date ON daily_statistics(user_id, date);
CREATE INDEX IF NOT EXISTS idx_consumed_entries_user_date ON consumed_entries(user_id, date);
CREATE INDEX IF NOT EXISTS idx_exercise_entries_user_date ON exercise_entries(user_id, date);
CREATE INDEX IF NOT EXISTS idx_emotion_entries_user ON emotion_entries(user_id);
