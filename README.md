
# Photoshop CLI

Photoshop CLI je jednoduchý nástroj pro úpravu obrázků ve formátu TGA pomocí příkazové řádky. Tento program umožňuje manipulaci s obrázky, jako je změna velikosti, ořezávání, posun a další operace.

---

## **Funkce programu**

Program podporuje následující příkazy:

- **resize w,h**  
  Změní velikost obrázku na šířku `w` a výšku `h`.

- **crop x,y,w,h**  
  Ořízne obrázek na obdélníkový výřez se začátkem v `(x, y)` a rozměry `w` a `h`.

- **move x,y**  
  Posune obrázek o `x` pixelů horizontálně a `y` pixelů vertikálně.

- **copy x1,y1,w,h x2,y2**  
  Zkopíruje obdélníkový výřez s počátkem v `(x1, y1)` a rozměry `w, h` na pozici `(x2, y2)`.

- **bw**  
  Převede obrázek na stupně šedi.

- **info**  
  Vypíše informace o aktuálním obrázku, jako je šířka, výška, počet kanálů apod.

- **save**  
  Uloží aktuální stav obrázku do výstupního souboru.

- **exit**  
  Uloží obrázek a ukončí program.

- **help**  
  Vypíše seznam dostupných příkazů.

---

## **Instalace a spuštění**

### **1. Klonování projektu**

Zkopírujte projekt do svého počítače:
```bash
git clone <url projektu>
cd photoshop-cli
```

### **2. Kompilace**

Použijte příkaz `make` k překladu programu:
```bash
make
```

Pokud `make` není k dispozici, použijte přímo `gcc`:
```bash
gcc main.c photoshop.c -o photoshop
```

---

### **3. Spuštění programu**

Spusťte program s parametry:
```bash
./photoshop --input <vstupní_soubor.tga> --output <výstupní_soubor.tga>
```

Příklad:
```bash
./photoshop --input tgaobrazek.tga --output output1.tga
```

---

## **Příklad relace**

Po spuštění programu zadejte příkazy pro úpravy obrázků. Příklad relace:

```bash
$ ./photoshop --input tgaobrazek.tga --output output1.tga
Image loaded successfully.
Image Info:
  Width: 512
  Height: 512
  Channels: 3
  Bits per pixel: 24
  Image type: 2

> resize 256,256
Image resized to 256x256.

> crop 10,10,100,100
Image cropped to region (10,10,100,100).

> move 20,-10
Image moved by (20,-10).

> copy 0,0,50,50 100,100
Region (0,0,50,50) copied to (100,100).

> bw
Image converted to grayscale.

> save
Image saved to 'output1.tga'.

> exit
Image saved to 'output1.tga'. Exiting.
```

---

## **Struktura projektu**

- **`main.c`**: Obsahuje hlavní logiku programu a interakci s uživatelem.
- **`photoshop.c`**: Obsahuje implementace funkcí pro manipulaci s TGA obrázky.
- **`structure.h`**: Hlavičkový soubor s deklaracemi struktur a funkcí.
- **`Makefile`**: Automatizace překladu programu.
- **`tgaobrazek.tga`**: Vstupní obrázek ve formátu TGA.
- **`output1.tga`**: Výstupní obrázek po úpravách.

---


