/**
 * @file
 * @author Atsushi Yokoyama <yokoyama@flogics.com>
 *
 * @section DESCRIPTION
 *
 * Main Routine File
 */

#define VERBOSE		1
#define LEN_BUF		64
#define TIMEOUT		10000
#define USB_VID		0x04D8
#define USB_PID		0x0053

#include <stdio.h>
#include <stdlib.h>		// exit()
#include <unistd.h>		// sleep()
#include "libusb.h"

unsigned char buf_in[LEN_BUF];
unsigned char buf_out[LEN_BUF];

/**
 * Main routine
 */
int main(int argc, char **argv)
{
	int status;
	int num_bytes_received;
	int num_bytes_sent;
	libusb_device_handle *device = NULL;

	if ((status = libusb_init(NULL))) {
		fprintf(stderr, "libusb_init() failed.\n");
		exit(2);
	}

	libusb_set_debug(NULL, VERBOSE);

	if ((device = libusb_open_device_with_vid_pid(
			NULL, (uint16_t)USB_VID, (uint16_t) USB_PID)) == NULL) {
		fprintf(stderr, "libusb_open_device_with_vid_pid() failed.\n");
		fprintf(stderr, "Check if USB target is connected to PC.\n");
		exit(2);
	}

	// This may return error except for Linux
	libusb_set_auto_detach_kernel_driver(device, 1);

	if ((status = libusb_claim_interface(device, 0))) {
		fprintf(stderr, "libusb_claim_interface() failed (status = %d).\n",
			status);
		exit(2);
	}
	printf("Target connected.\n");

	for (;;) {
		printf("Toggling LED.\n");

		buf_out[0] = 0x80;

		// Send a 1 byte bulk data transfer to EP1 OUT
		if ((status = libusb_bulk_transfer(
				device, 0x01, &buf_out[0], 1, &num_bytes_sent, TIMEOUT))) {
			fprintf(stderr, "libusb_bulk_transfer() failed.\n");
			fflush(stderr);
			exit(2);
		}

		buf_out[0] = 0x81;

		//Send a 1 byte bulk data transfer to EP1 OUT
		if ((status = libusb_bulk_transfer(
				device, 0x01, &buf_out[0], 1, &num_bytes_sent, TIMEOUT))) {
			fprintf(stderr, "libusb_bulk_transfer() failed.\n");
			fflush(stderr);
			exit(2);
		}

		//Request to read 64 bytes of bulk data from EP1 IN
		if ((status = libusb_bulk_transfer(
				device, 0x81, &buf_in[0], 64, &num_bytes_received, TIMEOUT))) {
			fprintf(stderr, "libusb_bulk_transfer() failed.\n");
			fflush(stderr);
			exit(2);
		}

		if (buf_in[0] == 0x81)
			printf("button status = %d\n", buf_in[1]);

		sleep(1);
	}

	return 0;
}
