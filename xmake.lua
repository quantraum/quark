set_runtimes(is_mode("debug") and "MDd" or "MD")
add_requires("catch2", "python", "nlohmann_json", "stb")
set_languages("c++20")
set_policy("build.ccache", false)

add_rules("mode.debug", "mode.release", "mode.coverage")

--
local function try_command(func, max_attempts)
    local attempt = 1

    while attempt <= max_attempts do
      local exit_code = func()
      if exit_code == 0 then
        return true
      else
        print("Attempt #" .. attempt .. " failed")
        attempt = attempt + 1
      end
    end

    return false
end


target("quark", function()
    set_kind("static")
    add_files("src/Quark/**.cpp")
    add_packages("nlohmann_json", { public = true })
    add_packages("stb")
    add_headerfiles("Src/(Quark/*.h)", { prefixdir = "Quark" })
    set_targetdir("./build/$(plat)/$(arch)/$(mode)/lib")

    after_build(function (target)
        os.mkdir(path.join(target:targetdir(), "../include/Quark"))
        os.cp("Src/Quark/**", path.join(target:targetdir(), "../include/Quark"))
        os.rm(path.join(target:targetdir(), "../include/Quark/**.cpp"))
    end)
end)

target("pyquark", function()
    set_kind("shared") -- Should be discarded but still present to generate CMakelists.txt on CLion
    add_deps("quark")
    add_packages("python")
    add_packages("nlohmann_json")
    add_files("src/PyQuark/Quark.cpp")
    set_targetdir("./build/$(plat)/$(arch)/$(mode)/python")
    add_includedirs("src")

    after_build(function (target)
        local njson = target:pkg("nlohmann_json")
        local nj_include = path.join(njson:installdir(), "include")

        build_dir = path.join(target:targetdir())
        os.cp(nj_include, path.join(build_dir, "nlohmann_json"))
        os.cp("src/PyQuark/*", build_dir)

        local function python_setup()
            return os.exec("pip install --no-cache-dir " .. build_dir)
        end

        try_command(python_setup, 1)
        -- local status, result = pcall(os.execute, "pip install --no-cache-dir " .. build_dir)

        os.rm(path.join(build_dir, "**.cpp"))
        os.rm(path.join(build_dir, "**.h"))
        os.rm(path.join(build_dir, "setup.py"))
        os.rm(path.join(build_dir, "nlohmann_json"))
    end)
end)

target("quark-sample", function()
    set_kind("binary")
    set_optimize("fastest")
    add_deps("quark")
    add_includedirs("src/Quark")
    add_files("src/QuarkSample/**.cpp")
    set_targetdir("./build/$(plat)/$(arch)/$(mode)/bin")

    after_build(function (target)
        os.cp("Datasets", path.join(target:targetdir()))
    end)
end)

target("quark-test", function()
    set_kind("binary")
    add_deps("quark")
    add_packages("catch2")
    add_includedirs("src")
    add_files("src/QuarkTest/**.cpp")
    set_targetdir("./build/$(plat)/$(arch)/$(mode)/bin")
end)
