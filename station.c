#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

void main(void)
{
	
	int i, fd, ret;

	char ch, writeBuff[100], readBuff[100];
	int writeInt1, writeInt2, readInt;

	fd = open("/dev/unionstation", O_RDWR);

	if (fd == -1)
	{
		printf ("Error in opening file %d\n", errno);
		exit(-1);
	}
	printf("Press r to read from device or w to write the device\n");
	scanf( "%c", &ch);

	switch(ch)
	{
		case 'w':
			printf("Enter the data to be written into the device \n");
			scanf("%d", &writeInt1);
			printf("Recieved %d\n", writeInt1);
			write(fd, &writeInt1, sizeof(int));
			break;
		case 'r':
			ret = read(fd, &readInt, sizeof(int));
			printf("The data in the device is %d\n", readInt );
			printf("Return value %d, %d\n", ret, errno);
			break;
		default:
			printf("Wrong choice\n");
			break;
	}
	close(fd);
}