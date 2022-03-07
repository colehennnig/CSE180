CREATE OR REPLACE FUNCTION
changeSomeAmountPaidFunction(theCustID INTEGER, maxReduction INTEGER)
RETURNS INTEGER AS $$


    DECLARE
    	amountReduced	INTEGER;
        theAmountPaid	INTEGER;
        thePaymentID	INTEGER;

    DECLARE paymentsCursor CURSOR FOR
    	    SELECT amountPaid, paymentID
            FROM Payments p, PaymentMethods m, Customers c
            WHERE c.custID = theCustID
               AND m.custID = theCustID
               AND p.cardType = m.cardType
               AND p.cardNum = m.cardNum
            ORDER BY paymentDate DESC, amountPaid DESC;

    BEGIN

	IF maxReduction <= 0 THEN
	    RETURN -1;
	    END IF;

        IF theCustID NOT IN (SELECT custID FROM Customers) THEN
            RETURN -2;
            END IF;

        amountReduced := 0;

        OPEN paymentsCursor;

        LOOP
 
            FETCH paymentsCursor INTO theAmountPaid, thePaymentID;

            EXIT WHEN NOT FOUND OR amountReduced >= maxReduction;

            IF theAmountPaid >= 100 AND maxReduction >= amountReduced + 5 THEN
                UPDATE Payments SET amountPaid = amountPaid - 5 WHERE paymentID = thePaymentID;
                amountReduced := amountReduced + 5;
            ELSIF theAmountPaid >= 50 AND theAmountPaid < 100 AND maxReduction >= amountReduced + 3 THEN
                UPDATE Payments SET amountPaid = amountPaid - 3 WHERE paymentID = thePaymentID;
                amountReduced := amountReduced + 3;
            ELSIF theAmountPaid >= 10 AND theAmountPaid < 50 AND maxReduction >= amountReduced + 1 THEN
                UPDATE Payments SET amountPaid = amountPaid - 1 WHERE paymentID = thePaymentID;
                amountReduced := amountReduced + 1;
            END IF;

        END LOOP;
        CLOSE paymentsCursor;

	RETURN amountReduced;

    END

$$ LANGUAGE plpgsql;
