#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "gen_board_conf.h"

/* It's conf/board.conf.h */
#include <board.conf.h>

static int gen_dev_enabled(const char *dev_name) {
	char buf[128];
	char def[64];

	sprintf(def, "#define CONF_%s_ENABLED", dev_name);
	sprintf(buf, "%-50s %d", def, 1);
	printf("%s\n", buf);
}

static int gen_field_int(const char *dev_name,
		const char *prop_name, const struct field_int *f) {
	char buf[128];
	char def[64];

	if (!f->name) {
		return -1;
	}

	if (!strcmp(f->name, "")) {
		sprintf(def, "#define CONF_%s_%s",
			dev_name, prop_name);
	} else {
		sprintf(def, "#define CONF_%s_%s_%s",
			dev_name, prop_name, f->name);
	}
	sprintf(buf, "%-50s %s", def, f->val);
	printf("%s\n", buf);

	return 0;
}

static int gen_field_func(const char *dev_name,
		const char *prop_name, const struct field_int *f) {
	char buf[128];
	char def[64];

	if (!f->name) {
		return -1;
	}

	if (!strcmp(f->name, "")) {
		sprintf(def, "#define CONF_%s_%s()",
			dev_name, prop_name);
	} else {
		sprintf(def, "#define CONF_%s_%s_%s()",
			dev_name, prop_name, f->name);
	}
	sprintf(buf, "%-50s %s", def, f->val);
	printf("%s\n", buf);

	return 0;
}

static int gen_field_pin(const char *dev_name,
		const char *prop_name, const struct field_pin *f) {
	char buf[128];
	char def[64];

	if (!f->name) {
		return -1;
	}

	sprintf(def, "#define CONF_%s_%s_%s_PORT",
		dev_name, prop_name, f->name);
	sprintf(buf, "%-50s %s", def, f->port);
	printf("%s\n", buf);

	sprintf(def, "#define CONF_%s_%s_%s_NR",
		dev_name, prop_name, f->name);
	sprintf(buf, "%-50s %s", def, f->n);
	printf("%s\n", buf);

	sprintf(def, "#define CONF_%s_%s_%s_AF",
		dev_name, prop_name, f->name);
	sprintf(buf, "%-50s %s", def, f->af);
	printf("%s\n", buf);

	return 0;
}

static int gen_field_gpio_out(const char *dev_name,
		const char *prop_name, const struct field_pin *f) {
	char buf[128];
	char def[64];

	if (!f->name) {
		return -1;
	}

	sprintf(def, "#define CONF_%s_%s_%s_PORT",
		dev_name, prop_name, f->name);
	sprintf(buf, "%-50s %s", def, f->port);
	printf("%s\n", buf);

	sprintf(def, "#define CONF_%s_%s_%s_NR",
		dev_name, prop_name, f->name);
	sprintf(buf, "%-50s %s", def, f->n);
	printf("%s\n", buf);

	printf("%s\n", buf);

	return 0;
}

int main() {
	int i, j;
	struct conf_item *uart_conf = &board_config[UART_IDX];
	const struct uart_conf *uart;
	struct conf_item *spi_conf = &board_config[SPI_IDX];
	const struct spi_conf *spi;
	struct conf_item *pwm_conf = &board_config[PWM_IDX];
	const struct pwm_conf *pwm;
	struct conf_item *led_conf = &board_config[LED_IDX];
	const struct led_conf *led;

	config();

	printf("#ifndef BOARD_CONFIG_H_\n");
	printf("#define BOARD_CONFIG_H_\n\n");

	/* UART */
	for (i = 0; i < uart_conf->array_size; i++) {
		uart = &((const struct uart_conf *) uart_conf->ptr)[i];

		if (uart->status != ENABLED) {
			continue;
		}

		gen_dev_enabled(uart->name);

		for (j = 0; j < ARRAY_SIZE(uart->dev.irqs); j++) {
			if (gen_field_int(uart->name,
					"IRQ", &uart->dev.irqs[j]) != 0) {
				break;
			}
		}

		for (j = 0; j < ARRAY_SIZE(uart->dev.pins); j++) {
			if (gen_field_pin(uart->name,
					"PIN", &uart->dev.pins[j]) != 0) {
				break;
			}
		}

		for (j = 0; j < ARRAY_SIZE(uart->dev.clocks); j++) {
			if (gen_field_func(uart->name,
					"CLK_ENABLE", &uart->dev.clocks[j]) != 0) {
				break;
			}
		}

		printf("\n");
	}

	/* SPI */
	for (i = 0; i < spi_conf->array_size; i++) {
		spi = &((const struct spi_conf *) spi_conf->ptr)[i];

		if (spi->status != ENABLED) {
			continue;
		}

		gen_dev_enabled(spi->name);

		for (j = 0; j < ARRAY_SIZE(spi->dev.irqs); j++) {
			if (gen_field_int(spi->name,
					"IRQ", &spi->dev.irqs[j]) != 0) {
				break;
			}
		}

		for (j = 0; j < ARRAY_SIZE(spi->dev.pins); j++) {
			if (gen_field_pin(spi->name,
					"PIN", &spi->dev.pins[j]) != 0) {
				break;
			}
		}

		for (j = 0; j < ARRAY_SIZE(spi->dev.clocks); j++) {
			if (gen_field_func(spi->name,
					"CLK_ENABLE", &spi->dev.clocks[j]) != 0) {
				break;
			}
		}

		printf("\n");
	}

	/* PWM */
	for (i = 0; i < pwm_conf->array_size; i++) {
		pwm = &((const struct pwm_conf *) pwm_conf->ptr)[i];

		gen_dev_enabled(pwm->name);
		gen_field_func(pwm->name, "CHANNEL", &pwm->channel);
		gen_field_func(pwm->name, "TIM", &pwm->instance);
		gen_field_int(pwm->name, "SERVO_POS", &pwm->servo_low);
		gen_field_int(pwm->name, "SERVO_POS", &pwm->servo_high);

		for (j = 0; j < ARRAY_SIZE(pwm->dev.pins); j++) {
			if (gen_field_pin(pwm->name,
					"PIN", &pwm->dev.pins[j]) != 0) {
				break;
			}
		}

		for (j = 0; j < ARRAY_SIZE(pwm->dev.clocks); j++) {
			if (gen_field_func(pwm->name,
					"CLK_ENABLE", &pwm->dev.clocks[j]) != 0) {
				break;
			}
		}

		printf("\n");
	}

	/* LED */
	for (i = 0; i < led_conf->array_size; i++) {
		led = &((const struct led_conf *) led_conf->ptr)[i];

		gen_dev_enabled(led->name);
		gen_field_func(led->name, "GPIO_PORT", &led->port);
		gen_field_func(led->name, "GPIO_PIN", &led->pin);

		printf("\n");
	}

	printf("#endif /* BOARD_CONFIG_H_ */\n");

	return 0;
}
