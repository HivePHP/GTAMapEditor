from pathlib import Path
import traceback

def main():
    # Папка, где находится этот Python-файл
    script_dir = Path(__file__).resolve().parent

    # Здесь будет создан GTAMapEditor
    project_root = script_dir / "GTAMapEditor"

    files = [
        "CMakeLists.txt",

        "src/main.cpp",

        "src/Core/Application.h",
        "src/Core/Application.cpp",

        "src/Platform/Win32Window.h",
        "src/Platform/Win32Window.cpp",

        "src/Platform/OpenGLContext.h",
        "src/Platform/OpenGLContext.cpp",

        "src/Math/Vector3.h",
        "src/Math/Matrix4.h",

        "src/Renderer/Renderer.h",
        "src/Renderer/Renderer.cpp",

        "src/Renderer/Camera.h",
        "src/Renderer/Camera.cpp",
    ]

    print("=" * 70)
    print("GTA MAP EDITOR - PROJECT CREATOR")
    print("=" * 70)

    print()
    print("Python файл:")
    print(script_dir)

    print()
    print("Папка проекта:")
    print(project_root)

    print()
    print("Создание структуры...")
    print("-" * 70)

    # Создаём корневую папку
    project_root.mkdir(parents=True, exist_ok=True)

    created = 0
    existing = 0

    for relative_file in files:

        file_path = project_root / relative_file

        # Создаём папку файла
        file_path.parent.mkdir(
            parents=True,
            exist_ok=True
        )

        # Если файл уже существует — не трогаем
        if file_path.exists():
            print("[УЖЕ ЕСТЬ] " + relative_file)
            existing += 1
            continue

        # Создаём файл
        file_path.write_text(
            "",
            encoding="utf-8"
        )

        print("[СОЗДАН]    " + relative_file)
        created += 1

    print("-" * 70)

    print()
    print("ГОТОВО!")
    print()
    print(f"Создано файлов : {created}")
    print(f"Уже существовало: {existing}")
    print()
    print("Путь проекта:")
    print(project_root)
    print()

    # Показываем итоговую структуру
    print("=" * 70)
    print("СТРУКТУРА")
    print("=" * 70)

    print("GTAMapEditor/")

    for path in files:
        print("├── " + path)

    print()
    print("=" * 70)


if __name__ == "__main__":

    try:
        main()

    except Exception:
        print()
        print("=" * 70)
        print("ОШИБКА!")
        print("=" * 70)
        print()

        traceback.print_exc()

        print()
        print("=" * 70)

    finally:
        print()
        input("Нажмите ENTER для закрытия...")