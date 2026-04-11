#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>


int	main(int argc, char **argv)
{
	t_args	args;

	if (parse_args(argc, argv, &args) == -1)
		return (1);
	printf("number_of_coders           : %d\n", args.number_of_coders);
	printf("time_to_burnout            : %d\n", args.time_to_burnout);
	printf("time_to_compile            : %d\n", args.time_to_compile);
	printf("time_to_debug              : %d\n", args.time_to_debug);
	printf("time_to_refactor           : %d\n", args.time_to_refactor);
	printf("number_of_compiles_required: %d\n", args.number_of_compiles_required);
	printf("dongle_cooldown            : %d\n", args.dongle_cooldown);
	printf("scheduler                  : %s\n", args.scheduler == FIFO ? "fifo" : "edf");
	return (0);

}
