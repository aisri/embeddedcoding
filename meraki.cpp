#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
Let's pretend we're building the firmware for a brand-new system
on a chip. The manufacturer has supplied us with a "device tree",
which describes what peripherals are available to the processor
(see the definition of `device_root` below).

Please implement `enumerate_devices`. It should print something
that looks like this (note that indentation is important):

root
    gpbus
        gp0
        gp1
        gp2
    m1
        m2
            data
            prog
    nv
*/

/*
The manufacturer has sent us some new documentation. It explains
how to interpret the `info` value:

-- BEGIN SNIPPET --

15 14 | 13 12 11 10 9  | 8  7  6  5 | 4  3  2  1  0
type  | param          | attrib     | unused
type: 0=”flash” 1=”memory” 2=”gpio” 3=”bus”
param:
    when type=”flash”  .. number of 1kB pages
         type=”memory” .. total size in kB
         type=”gpio”   .. pin number
         type=”bus”    .. no meaning
attrib:
    when type=”flash”  .. no meaning
    when type=”memory” ..
         bit 8 .. 1=readable
         bit 7 .. 1=writable
         bit 6 .. 1=executable
         bit 5 .. 0=word-aligned, 1=dword-aligned
    when type=”gpio” ..
         bit 8 .. 1=input-capable
         bit 7 .. 1=output-capable
         bit 6, 5 .. unused
    when type=”bus” .. no meaning

-- END SNIPPET --

Starting with your implementation of `enumerate_devices`, please
implement `enumerate_devices_ext`. It addition to printing each
device's name (as before), if the device is a *memory device*, it
should print any information we have about the memory device.
*/

/*
Your coworkers need a function that helps them find GPIOs.

For example:

find_gpios(1, 1, -1, callback_fn)

    .. should call callback_fn(device) for each GPIO
       that can be either an input or an output.

find_gpios(1, 0, -1, callback_fn)

    .. should call callback_fn(device) for each GPIO
       that can be an input.

find_gpios(1, 0, 4, callback_fn)

    .. should call callback_fn() for the GPIO on pin 4, but only
       if it can act as an input.
*/

typedef struct device {
    const char* name;
    struct device* next_sibling;
    struct device* first_child;
    uint16_t info;
} device_t;

device_t device_prog = {
    .name = "prog",
    .next_sibling = NULL,
    .first_child = NULL,
    .info = 0b0100100101000000
};

device_t device_data = {
    .name = "data",
    .next_sibling = &device_prog,
    .first_child = NULL,
    .info = 0b0110000110000000
};

device_t device_m2 = {
    .name = "m2",
    .next_sibling = NULL,
    .first_child = &device_data,
    .info = 0b1100000000000000
};

device_t device_gp2 = {
    .name = "gp2",
    .next_sibling = NULL,
    .first_child = NULL,
    .info = 0b1000011010000000
};

device_t device_gp1 = {
    .name = "gp1",
    .next_sibling = &device_gp2,
    .first_child = NULL,
    .info = 0b1000010100000000
};

device_t device_gp0 = {
    .name = "gp0",
    .next_sibling = &device_gp1,
    .first_child = NULL,
    .info = 0b1000001110000000
};

device_t device_nv = {
    .name = "nv",
    .next_sibling = NULL,
    .first_child = NULL,
    .info = 0b0010000000000000
};

device_t device_m1 = {
    .name = "m1",
    .next_sibling = &device_nv,
    .first_child = &device_m2,
    .info = 0b1100000000000000
};

device_t device_gpbus = {
    .name = "gpbus",
    .next_sibling = &device_m1,
    .first_child = &device_gp0,
    .info = 0b1100000000000000
};

device_t device_root = {
    .name = "root",
    .next_sibling = NULL,
    .first_child = &device_gpbus,
    .info = 0
};

enum device_type {
    FLASH = 0,
    MEMORY = 1,
    GPIO = 2,
    BUS = 3,
    UNK = 4
};
#define DEVICE_TYPE_MASK (0xC000)
#define DEVICE_TYPE_SHIFT (0xE)
#define IS_MEMORY(x) (((x)&DEVICE_TYPE_MASK) == (MEMORY << DEVICE_TYPE_SHIFT))
#define IS_GPIO(x) (((x)&DEVICE_TYPE_MASK) == (GPIO << DEVICE_TYPE_SHIFT))

enum memory_attribs {
    READ = 0x8,
    WRITE = 0x7,
    EXEC = 0x6,
    WORD_ALIGN = 0x5
};

#define DEVICE_ATTRIB_MASK (0x1E0)
#define IS_READ(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << READ))
#define IS_WRITE(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << WRITE))
#define IS_EXEC(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << EXEC))
#define IS_WORD_ALIGN(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << WORD_ALIGN))

enum gpio_attribs {
    INPUT = 0x8,
    OUTPUT = 0x7,
};
#define IS_INPUT(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << INPUT))
#define IS_OUTPUT(x) (((x) & DEVICE_ATTRIB_MASK) && (0x1 << OUTPUT))

#define IDENT (" ")

typedef void (*gpio_cb)(device_t*);
typedef struct gpio_args {
    bool is_inp;
    bool is_out;
    int which;
    gpio_cb cb;
} gpio_args;
#define GPIO_PIN_MASK (0x3E00)
#define GPIO_PIN_SHIFT (0x8)
#define GPIO_PIN_NUMBER(x) (((x) & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT)

void enumerate_devices(device_t* root, int level = 0, bool print = false, gpio_args* args = NULL)
{
    if (!root) {
        return;
    }

    for (int lvl = 0; lvl < level; lvl++)
        printf(IDENT);

    printf("%s", root->name);
    if (print || args) {
        printf(" (raw:0x%X) ", root->info);
        if (IS_MEMORY(root->info)) {
            if (IS_READ(root->info)) {
                printf("R ");
            }
            if (IS_WRITE(root->info)) {
                printf("W ");
            }
            if (IS_EXEC(root->info)) {
                printf("X ");
            }
            if (IS_WORD_ALIGN(root->info)) {
                printf("WA ");
            } else {
                printf("DA ");
            }
        } else if (IS_GPIO(root->info)) {
            gpio_args temp;

            if (IS_INPUT(root->info)) {
                printf("INP ");
                temp.is_inp = true;
            }
            if (IS_OUTPUT(root->info)) {
                printf("OUT ");
                temp.is_out = true;
            }

            int tmp = temp.is_inp | temp.is_out << 1;
            if (args) {
                temp.which = GPIO_PIN_NUMBER(root->info);
                if (args->which == -1)
                    temp.which = -1;

                if (args->which == temp.which
                    && (tmp & args->is_inp
                        || tmp & args->is_out)) {
                    args->cb(root);
                }
            }
        }
    }

    printf("\n");

    // first process child node
    enumerate_devices(root->first_child, level + 1, print, args);

    // next process siblings
    enumerate_devices(root->next_sibling, level, print, args);
}

void enumerate_devices_ext(device_t* root)
{
    enumerate_devices(root, 0, true, NULL);
}

void process_gpio(device_t* dev)
{
    printf("*** executing %s", dev->name);
}

void find_gpios(int is_input, int is_output, int which_pin, gpio_cb cb)
{
    gpio_args args = {
        .is_inp = is_input,
        .is_out = is_output,
        .which = which_pin,
        .cb = cb
    };
    enumerate_devices(&device_root, 0, false, &args);
}

int main()
{
    printf("Enumerating Devices\n");
    enumerate_devices(&device_root);
    printf("\nEnumerating Devices W/ Extensions\n");
    enumerate_devices_ext(&device_root);
    printf("\nFinding GPIOs\n");
    printf("\nfind_gpios(1, 1, -1, process_gpio);\n");
    find_gpios(1, 1, -1, process_gpio);
    printf("\nfind_gpios(1, 0, -1, process_gpio);\n");
    find_gpios(1, 0, -1, process_gpio);
    printf("\nfind_gpios(0, 1, -1, process_gpio);\n");
    find_gpios(0, 1, -1, process_gpio);
    printf("\nfind_gpios(1, 0, 4, process_gpio);\n");
    find_gpios(1, 0, 4, process_gpio);
    return 0;
}