1. Get Arknights data dump and put characters into `data/` directory
2. Get spine-runtime 3.8 branch (because Arknights uses that)
3. Copy `spine-cpp/` and `spine-sfml/` folders into this directory
4. `meson setup build`
5. `cd build/`
6. `ninja`
7. `cd ..`
8. `build/arknights`
