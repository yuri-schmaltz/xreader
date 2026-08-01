#include <config.h>
#include <glib.h>

#include "ev-form-field.h"

static void
test_form_field_stub (void)
{
	g_assert_true (TRUE);
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/form-field/stub", test_form_field_stub);

	return g_test_run ();
}
