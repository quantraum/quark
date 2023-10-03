from distutils.core import Extension, setup


def main():
    setup(
        name="quark",
        version="0.1.3",
        description="Python wrapper for the Quark library",
        author="Antoine Houet, Martin Salmon & Benjamin Wirtz",
        url="https://www.bedevin.com",
        ext_modules=[
            Extension(
                "quark",
                ["Quark.cpp"],
                libraries=["quark"],
                library_dirs=["../lib"],
                include_dirs=["../include", "./nlohmann_json"],
            )
        ],
    )


if __name__ == "__main__":
    main()
