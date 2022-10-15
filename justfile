build: setup
    ninja -C build

setup:
    meson setup build

clean:
    rm -r build

test: build
    meson test -C build --print-errorlogs
