-- Создание базы данных nutrition_app.db
-- SQLite database schema and data

-- Создаем таблицу для достижений
CREATE TABLE IF NOT EXISTS achievements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    unlocked BOOLEAN NOT NULL DEFAULT 0,
    description TEXT
);

-- Создаем таблицу для продуктов
CREATE TABLE IF NOT EXISTS products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL
);

-- Создаем таблицу для рецептов
CREATE TABLE IF NOT EXISTS recipes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL
);

-- Создаем таблицу для ингредиентов рецептов
CREATE TABLE IF NOT EXISTS recipe_ingredients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    recipe_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    quantity REAL NOT NULL,
    unit TEXT NOT NULL,
    proteins REAL NOT NULL,
    fats REAL NOT NULL,
    carbs REAL NOT NULL,
    FOREIGN KEY (recipe_id) REFERENCES recipes (id)
);

-- Создаем таблицу для пользователей
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE, -- имя уникальное
    age INTEGER NOT NULL,
    gender TEXT NOT NULL CHECK (gender IN ('Мужской', 'Женский')), -- только два значения
    height REAL NOT NULL,
    weight REAL NOT NULL,
    goal TEXT NOT NULL CHECK (goal IN ('Сбросить вес', 'Сохранить вес', 'Набрать вес')), -- ограниченный список целей
    activity_level INTEGER NOT NULL CHECK (activity_level BETWEEN 1 AND 4) -- от 1 до 4
);


-- Создаем таблицу для эмоций
CREATE TABLE IF NOT EXISTS emotions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    classification TEXT NOT NULL
);

-- Создаем таблицу для упражнений
CREATE TABLE IF NOT EXISTS exercises (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL
);

-- Создаем таблицу статистики по дням
CREATE TABLE IF NOT EXISTS daily_statistics (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    water_ml REAL NOT NULL DEFAULT 0,
    total_proteins REAL NOT NULL DEFAULT 0,
    total_fats REAL NOT NULL DEFAULT 0,
    total_carbs REAL NOT NULL DEFAULT 0,
    total_calories REAL NOT NULL DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES users (id),
    UNIQUE(user_id, date)
);

-- Создаем таблицу для записей об эмоциях
CREATE TABLE IF NOT EXISTS emotion_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    emotion_id INTEGER NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id),
    FOREIGN KEY (emotion_id) REFERENCES emotions (id)
);

-- Создаем таблицу для записей о потребленной еде
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
    FOREIGN KEY (user_id) REFERENCES users (id)
);

-- Создаем таблицу для записей об упражнениях
CREATE TABLE IF NOT EXISTS exercise_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    exercise_name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id)
);

-- Создаем таблицу для эмоций
CREATE TABLE IF NOT EXISTS emotions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    classification TEXT NOT NULL
);

-- Создаем таблицу для упражнений
CREATE TABLE IF NOT EXISTS exercises (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL
);

-- Создаем таблицу статистики по дням
CREATE TABLE IF NOT EXISTS daily_statistics (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    water_ml REAL NOT NULL DEFAULT 0,
    total_proteins REAL NOT NULL DEFAULT 0,
    total_fats REAL NOT NULL DEFAULT 0,
    total_carbs REAL NOT NULL DEFAULT 0,
    total_calories REAL NOT NULL DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES users (id),
    UNIQUE(user_id, date)
);

-- Создаем таблицу для записей об эмоциях
CREATE TABLE IF NOT EXISTS emotion_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    emotion_id INTEGER NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id),
    FOREIGN KEY (emotion_id) REFERENCES emotions (id)
);

-- Создаем таблицу для записей о потребленной еде
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
    FOREIGN KEY (user_id) REFERENCES users (id)
);

-- Создаем таблицу для записей об упражнениях
CREATE TABLE IF NOT EXISTS exercise_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    exercise_name TEXT NOT NULL,
    duration_minutes REAL NOT NULL,
    calories_burned REAL NOT NULL,
    timestamp TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id)
);

-- Вставляем данные в таблицу достижений
INSERT INTO achievements (name, unlocked, description) VALUES 
('Первый день', 0, 'Поздравляем! Вы используете приложение первый день.'),
('Первая неделя', 0, 'Отлично! Вы пользуетесь приложением уже неделю.'),
('Первый месяц', 0, 'Вы успешно использовали приложение в течение месяца.'),
('Мастер калорий', 0, 'Вы достигли своей дневной нормы калорий.'),
('Гид по воде', 0, 'Вы выпили рекомендованное количество воды за день.'),
('Эксперт рецептов', 0, 'Вы создали или использовали рецепты для приготовления блюд.'),
('Фитнес чемпион', 0, 'Вы сожгли более 1000 ккал за один день.');

-- Вставляем данные в таблицу продуктов
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

-- Вставляем данные в таблицу рецептов
INSERT INTO recipes (name, quantity, unit, proteins, fats, carbs) VALUES 
('Куриный суп с овощами', 100, 'порция', 50, 6, 25),
('Овсяная каша с фруктами', 100, 'порция', 11, 6, 89),
('Гречневая каша с творогом', 100, 'порция', 10, 3, 28),
('Бананово-яблочный смузи', 100, 'стакан', 1, 0, 37),
('Салат из брокколи с творогом', 100, 'порция', 18, 5, 19);

-- Вставляем ингредиенты для рецепта "Куриный суп с овощами" (recipe_id = 1)
INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) VALUES 
(1, 'Куриная грудка', 150, 'г', 47, 5, 0),
(1, 'Картофель', 100, 'г', 2, 0, 17),
(1, 'Морковь', 50, 'г', 0, 0, 5),
(1, 'Брокколи', 50, 'г', 1, 0, 4);

-- Вставляем ингредиенты для рецепта "Овсяная каша с фруктами" (recipe_id = 2)
INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) VALUES 
(2, 'Овсянка', 80, 'г', 9, 5, 53),
(2, 'Яблоко', 100, 'г', 0, 0, 14),
(2, 'Банан', 50, 'г', 1, 0, 22);

-- Вставляем ингредиенты для рецепта "Гречневая каша с творогом" (recipe_id = 3)
INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) VALUES 
(3, 'Гречка', 100, 'г', 3, 1, 20),
(3, 'Творог', 50, 'г', 7, 2, 2),
(3, 'Яблоко', 50, 'г', 0, 0, 7);

-- Вставляем ингредиенты для рецепта "Бананово-яблочный смузи" (recipe_id = 4)
INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) VALUES 
(4, 'Банан', 100, 'г', 1, 0, 23),
(4, 'Яблоко', 100, 'г', 0, 0, 14);

-- Вставляем ингредиенты для рецепта "Салат из брокколи с творогом" (recipe_id = 5)
INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) VALUES 
(5, 'Брокколи', 150, 'г', 4, 1, 11),
(5, 'Творог', 100, 'г', 14, 4, 3),
(5, 'Морковь', 50, 'г', 1, 0, 4);

-- Вставляем данные пользователя
INSERT INTO users (name, age, gender, height, weight, goal, activity_level) VALUES 
('User', 30, 'Мужской', 180, 75, 'Сохранить вес', 2);

-- Создаем индексы для оптимизации запросов
CREATE INDEX IF NOT EXISTS idx_daily_statistics_user_date ON daily_statistics(user_id, date);
CREATE INDEX IF NOT EXISTS idx_consumed_entries_user_date ON consumed_entries(user_id, date);
CREATE INDEX IF NOT EXISTS idx_exercise_entries_user_date ON exercise_entries(user_id, date);
CREATE INDEX IF NOT EXISTS idx_emotion_entries_user ON emotion_entries(user_id);
