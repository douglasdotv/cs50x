-- (1) Get Humphrey Street crime reports from July 28, 2021:
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street LIKE '%Humphrey%';

-- Results:
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.


-- (2) Get interview transcripts that mention the bakery:
SELECT transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28
AND transcript like '%bakery%';

-- Results:
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
--
-- I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
--
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- (3) Get people who were at the bakery, made a withdrawal from the ATM on Leggett Street and made a short phone call on July 28, 2021:
SELECT name AS suspect, phone_number, license_plate, passport_number
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    AND minute <= 25
    AND activity = 'exit'
)
INTERSECT
SELECT pp.name, pp.phone_number, pp.license_plate, pp.passport_number
FROM people pp
JOIN bank_accounts ba ON pp.id = ba.person_id
WHERE ba.account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
)
INTERSECT
SELECT name, phone_number, license_plate, passport_number
FROM people
WHERE phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
);

-- Results:
-- +---------+----------------+---------------+-----------------+
-- | suspect |  phone_number  | license_plate | passport_number |
-- +---------+----------------+---------------+-----------------+
-- | Bruce   | (367) 555-5533 | 94KL13X       | 5773159633      |
-- | Diana   | (770) 555-1861 | 322W7JE       | 3592750733      |
-- +---------+----------------+---------------+-----------------+


-- (4) Check if the suspects were present on the earliest flight out of Fiftyville on July 29, 2021:
SELECT pp.name AS passenger, pp.passport_number
FROM people pp
JOIN passengers pa ON pp.passport_number = pa.passport_number
WHERE pa.flight_id IN (
    SELECT fl.id
    FROM flights fl
    JOIN airports ap ON fl.origin_airport_id = ap.id
    WHERE ap.city = 'Fiftyville'
    AND fl.year = 2021
    AND fl.month = 7
    AND fl.day = 29
    ORDER BY fl.hour, fl.minute
    LIMIT 1
)
AND pa.passport_number IN (5773159633, 3592750733);

-- Results:
-- +-----------+-----------------+
-- | passenger | passport_number |
-- +-----------+-----------------+
-- | Bruce     | 5773159633      |
-- +-----------+-----------------+


-- (5) Get the city the thief escaped to:
SELECT ap1.city AS destination_city
FROM flights fl
JOIN airports ap1 ON fl.destination_airport_id = ap1.id
JOIN airports ap2 ON fl.origin_airport_id = ap2.id
WHERE ap2.city = 'Fiftyville'
AND fl.year = 2021
AND fl.month = 7
AND fl.day = 29
ORDER BY fl.hour, fl.minute
LIMIT 1;

-- Results:
-- +------------------+
-- | destination_city |
-- +------------------+
-- | New York City    |
-- +------------------+


-- (6) Find the accomplice:
SELECT pp.name AS accomplice
FROM people pp
JOIN phone_calls pc ON pp.phone_number = pc.receiver
WHERE pc.caller = '(367) 555-5533'
AND year = 2021
AND month = 7
AND day = 28
AND duration < 60;

-- Results:
-- +------------+
-- | accomplice |
-- +------------+
-- | Robin      |
-- +------------+
