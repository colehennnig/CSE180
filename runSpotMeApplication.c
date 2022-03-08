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
    char command[MAXSQLSTATEMENTSTRINGSIZE];
    sprintf(command, "SELECT name FROM Customers WHERE custID = %d;", theCustID);
    PGresult *res = PQexec(conn, command);
    if (PQntuples(res) < 1) {
        PQclear(res);
        return -1;
    }
    printf("Customer %d %s ", theCustID, PQgetvalue(res, 0, 0));
    PQclear(res);
    memset(command, 0, strlen(command));
    sprintf(command, "SELECT * FROM Customers C, PaymentMethods M, Payments P WHERE C.custID = %d AND M.custID = %d AND M.cardType = P.cardType AND M.cardNum = P.cardNum AND P.paymentDate > M.expirationDate;", theCustID, theCustID);
    res = PQexec(conn, command);
    printf("expired: %d ", PQntuples(res));
    PQclear(res);
    memset(command, 0, strlen(command));
    sprintf(command, "SELECT * FROM Customers C, PaymentMethods M, Payments P WHERE C.custID = %d AND M.custID = %d AND M.cardType = P.cardType AND M.cardNum = P.cardNum AND P.paymentDate <= M.expirationDate;", theCustID, theCustID);
    res = PQexec(conn, command);
    printf("unexpired: %d\n", PQntuples(res));
    PQclear(res);
    return 0;
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
    char command[MAXSQLSTATEMENTSTRINGSIZE];
    sprintf(command, "SELECT * FROM PaymentMethods WHERE cardType = '%s' AND cardNum = %d;", theCardType, theCardNum);
    PGresult *res = PQexec(conn, command);
    if (PQntuples(res) < 1) {
        return -1;
    }
    PQclear(res);
    memset(command, 0, strlen(command));
    sprintf(command, "SELECT isValid FROM PaymentMethods WHERE cardType = '%s' AND cardNum = %d AND CURRENT_DATE > expirationDate;", theCardType, theCardNum);
    res = PQexec(conn, command);
    if (PQntuples(res) < 1 || strcmp(PQgetvalue(res, 0, 0), "f") == 0) {
        return 0;
    }
    PQclear(res);
    memset(command, 0, strlen(command));
    sprintf(command, "UPDATE PaymentMethods SET isValid = 'FALSE' WHERE cardType = '%s' AND cardNum = %d;", theCardType, theCardNum);
    res = PQexec(conn, command);
    return 1;
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
    char command[MAXSQLSTATEMENTSTRINGSIZE];
    sprintf(command, "SELECT changeSomeAmountPaidFunction(%d, %d);", theCustID, maxReduction);
    PGresult *res = PQexec(conn, command);
    return atoi(PQgetvalue(res, 0, 0));
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
    result = printCustomerPaymentInfo(conn, 104);
    if (result == -1) {
        printf("No customer exists whose id is 104\n");
    } else if (result != 0) {
        printf("Error from function call: printCustomerPaymentInfo\n");
    }
    result = printCustomerPaymentInfo(conn, 102);
    if (result == -1) {
        printf("No customer exists whose id is 102\n");
    } else if (result != 0) {
        printf("Error from function call: printCustomerPaymentInfo\n");
    }
    /* Extra newline for readability */
    printf("\n");
    
    /* Perform the calls to updateIsValid listed in Section 6 of Lab4,
     * and print their results as described.
     */
    result = updateIsValid(conn, "M", 5380746);
    if (result == 1) {
        printf("Validity changed for cardType M and cardNum 5380746\n");
    } else if (result == 0) {
        printf("Validity not changed for cardType M and cardNum 5380746\n");
    } else if (result == -1) {
        printf("No such cardType M and cardNum 5380746\n");
    } else {
        printf("Error from function call: updateIsValid\n");
        bad_exit(conn);
    }
    result = updateIsValid(conn, "V", 6011024);
    if (result == 1) {
        printf("Validity changed for cardType V and cardNum 6011024\n");
    } else if (result == 0) {
        printf("Validity not changed for cardType V and cardNum 6011024\n");
    } else if (result == -1) {
        printf("No such cardType V and cardNum 6011024\n");
    } else {
        printf("Error from function call: updateIsValid\n");
        bad_exit(conn);
    }
    result = updateIsValid(conn, "M", 5380346);
    if (result == 1) {
        printf("Validity changed for cardType M and cardNum 5380346\n");
    } else if (result == 0) {
        printf("Validity not changed for cardType M and cardNum 5380346\n");
    } else if (result == -1) {
        printf("No such cardType M and cardNum 5380346\n");
    } else {
        printf("Error from function call: updateIsValid\n");
        bad_exit(conn);
    }

    /* Extra newline for readability */
    printf("\n");
    
    /* Perform the calls to changeSomeAmountPaid listed in Section 6 of Lab4,
     * and print their results as described.
     */
    result = changeSomeAmountPaid(conn, 104, 14);
    if (result >= 0) {
        printf("Total reduction for %d and maxReduction: is %d\n", 104, result);
    } else if (result == -1) {
        printf("Max reduction is negative\n");
    } else if (result == -2) {
        printf("CustID %d not found\n", 104);
    }
    result = changeSomeAmountPaid(conn, 100, 16);
    if (result >= 0) {
        printf("Total reduction for %d and maxReduction: is %d\n", 100, result);
    } else if (result == -1) {
        printf("Max reduction is negative\n");
    } else if (result == -2) {
        printf("CustID %d not found\n", 100);
    }
    result = changeSomeAmountPaid(conn, 20, 4);
    if (result >= 0) {
        printf("Total reduction for %d and maxReduction: is %d\n", 20, result);
    } else if (result == -1) {
        printf("Max reduction is negative\n");
    } else if (result == -2) {
        printf("CustID %d not found\n", 20);
    }
    result = changeSomeAmountPaid(conn, 104, 14);
    if (result >= 0) {
        printf("Total reduction for %d and maxReduction: is %d\n", 104, result);
    } else if (result == -1) {
        printf("Max reduction is negative\n");
    } else if (result == -2) {
        printf("CustID %d not found\n", 104);
    }

    good_exit(conn);
    return 0;
}
