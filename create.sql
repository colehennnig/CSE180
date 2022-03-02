
-- CSE 180 Winter 2022 Lab4 create.sql solution.


-- Following two lines are not needed in your solution.
DROP SCHEMA Lab4 CASCADE;
CREATE SCHEMA Lab4;


-- Customers(custID, name, streetAddr, city, state, joinDate, level, canReserve)
CREATE TABLE Customers(
    custID INT PRIMARY KEY,
    name VARCHAR(50) UNIQUE,
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    joinDate DATE,
    level CHAR, 
    canReserve BOOLEAN
);


--Vehicles(state, licensePlate, vehicleType, year, make, color, ownerID)
CREATE TABLE Vehicles(
    state CHAR(2),
    licensePlate CHAR(7),
    vehicleType CHAR NOT NULL,
    year INT,
    make VARCHAR(20),
    color CHAR,
    ownerID INT,
    PRIMARY KEY (state, licensePlate),
    FOREIGN KEY (ownerID) REFERENCES Customers(custID)
);


-- ParkingLots(parkingLotID, streetAddr, city, state, numSpots, costPerHour)
CREATE TABLE ParkingLots(
    parkingLotID INT PRIMARY KEY,
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    numSpots INT,
    costPerHour NUMERIC(5,2) NOT NULL,
    UNIQUE(streetAddr, city, state)
);


-- Reservations(reservationID, reserverID, parkingLotID, state, licensePlate, reservationDate,
--               startTime, endTime, actualArrivalTime, actualDepartureTime)
CREATE TABLE Reservations(
    reservationID INT PRIMARY KEY,
    reserverID INT,
    parkingLotID INT,
    state CHAR(2),
    licensePlate CHAR(7),
    reservationDate DATE,
    startTime TIME,
    endTime TIME,
    actualArrivalTime TIME,
    actualDepartureTime TIME,
    UNIQUE(reserverID, reservationDate),
    FOREIGN KEY (reserverID) REFERENCES Customers(custID),
    FOREIGN KEY (parkingLotID) REFERENCES ParkingLots(parkingLotID),
    FOREIGN KEY (state, licensePlate) REFERENCES Vehicles(state, licensePlate)
);


-- PaymentMethods(cardType, cardNum, custID, expirationDate, isValid)
CREATE TABLE PaymentMethods(
    cardType CHAR,
    cardNum INT,
    custID INT,
    expirationDate DATE NOT NULL,
    isValid BOOLEAN,
    PRIMARY KEY (cardType, cardNum),
    FOREIGN KEY (custID) REFERENCES Customers(custID)
);


-- Payments(paymentID, reservationID, cardType, cardNum, paymentDate, amountPaid)
CREATE TABLE Payments(
    paymentID INT PRIMARY KEY,
    reservationID INT,
    cardType CHAR,
    cardNum INT,
    paymentDate DATE,
    amountPaid NUMERIC(6,2),
    FOREIGN KEY (reservationID) REFERENCES Reservations(reservationID),
    FOREIGN KEY (cardType, cardNum) REFERENCES PaymentMethods(cardType, cardNum)
);
