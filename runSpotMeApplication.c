/**
 * runSupermarketApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define  MAXNUMBERSTRINGSIZE        20


/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 */

/* Function: printCustomerPaymentInfo:
 * -------------------------------------
 * Parameters:  connection, and thecustID, which should be the custID of a customer.
 * Prints custID, number of expired payments and number of unexpired payments.
 * Return 0 if normal execution, -1 if no such customer.
 * bad_exit if SQL statement execution fails.
 */


int printCustomerPaymentInfo(PGconn *conn, int theCustID)
{

}

/* Function: updateIsValid:
 * ----------------------------
 * Parameters:  connection, and theCardType and theCardNum which should correspond to a payment method.
 * Updates the isValid status of expired payment method if isValid for that
 * payment method isn't already FALSE.
 * Returns 1 if isValid was updated for the payment method, 0 if it wasn't updated, and -1 if the payment method doesn't exist.
 */

int updateIsValid(PGconn *conn, char *theCardType, int theCardNum)
{

}

/* Function: changeSomeAmountPaid:
 * -------------------------------
 * Parameters:  connection, theCustID which should be the custID of a customer
 * whose amountPaid values in Payments might be reduced, and maxReduction,
 * the maximum total reduction of amountPaid values.
 * Executes by invoking a Stored Function, changeSomeAmountPaidFunction, which
 * returns a negative if there is an error, and otherwise returns the total
 * reduction performed by the Stored Function.
 */

int changeSomeAmountPaid(PGconn *conn, int theCustID, int maxReduction)
{
    
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runSpotMeApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }

    int result;
    
    /* Perform the calls to printCustomerPaymentInfo listed in Section 6 of Lab4,
     * and print their results as described.
     */
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to updateIsValid listed in Section 6 of Lab4,
     * and print their results as described.
     */
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to changeSomeAmountPaid listed in Section 6 of Lab4,
     * and print their results as described.
     */

    good_exit(conn);
    return 0;
}
