-- Keep a log of any SQL queries you execute as you solve the mystery.

-- finds all crime scene reports from the date of the crime
select description from crime_scene_reports
where month = 7 AND day = 28 AND year = 2021;
-- returned the following:
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time –
-- each of their interview transcripts mentions the bakery.

select * from interviews
where month = 7 AND day = 28 AND year = 2021
AND transcript like "%bakery%";

-- | 161 | Ruth    | 2021 | 7     | 28  |
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery
-- parking lot and drive away. If you have security footage from the bakery parking lot,
-- you might want to look for cars that left the parking lot in that time frame.

-- looking into ruth's account of what happened
select * from bakery_security_logs
where month = 7 AND day = 28 AND year = 2021
AND hour = 10 AND minute >= 5 AND minute <=25
AND activity = "exit";
-- found 8 suspect license plates that left the parking lot within 10 minutes of the crime

select name from people, bakery_security_logs
where bakery_security_logs.license_plate = people.license_plate
and month = 7 AND day = 28 AND year = 2021
AND hour = 10 AND minute >= 5 AND minute <=25
AND activity = "exit";


-- | 162 | Eugene  | 2021 | 7     | 28  |
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning,
-- before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw
-- the thief there withdrawing some money.

-- looking into Eugene's account
select * from atm_transactions
where atm_location = "Leggett Street"
AND transaction_type = 'withdraw' AND month = 7 AND day = 28 AND year = 2021;
--- found 8 accoutns that withdrew money from the leggett st atm on the 28th

select name from people, atm_transactions, bank_accounts
where atm_transactions.account_number = bank_accounts.account_number
and bank_accounts.person_id = people.id
and atm_location = "Leggett Street"
AND transaction_type = 'withdraw' AND month = 7 AND day = 28 AND year = 2021;
-- the names of the 8 individuals that withdrew money from the leggett st atm on the 28th


-- | 163 | Raymond | 2021 | 7     | 28  |
-- As the thief was leaving the bakery, they called someone who talked to them for less than
-- a minute. In the call, I heard the thief say that they were planning to take the earliest
-- flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the
-- phone to purchase the flight ticket.

--looking into Raymond's account

-- finds the phone numbers that made a call for less than a minute the day of the crime
select name from people, phone_calls
where people.phone_number = phone_calls.caller
and month = 7 AND day = 28 AND year = 2021
and duration <= 60;

select * from flights, airports
where airports.id = flights.origin_airport_id
and origin_airport_id = (select id from airports where city = "Fiftyville")
AND month = 7 AND day = 29 AND year = 2021
ORDER BY hour asc, minute asc
limit 1;
-- finds all the information available on the earliest flight out of fiftyville airport on the 29th (day after the crime)

-- CITY OF ESCAPE
select city from airports
where id = (select destination_airport_id from flights, airports
where airports.id = flights.origin_airport_id
and origin_airport_id = (select id from airports where city = "Fiftyville")
AND month = 7 AND day = 29 AND year = 2021
ORDER BY hour asc, minute asc
limit 1);
-- determines the city the criminals escaped to

select passport_number from passengers
where flight_id = (select flights.id from flights, airports
where airports.id = flights.origin_airport_id
and origin_airport_id = (select id from airports where city = "Fiftyville")
AND month = 7 AND day = 29 AND year = 2021
ORDER BY hour asc, minute asc
limit 1);
-- finds the passport numbers of all people on the flight

select name from people
where passport_number in (
select passport_number from passengers
where flight_id = (select flights.id from flights, airports
where airports.id = flights.origin_airport_id
and origin_airport_id = (select id from airports where city = "Fiftyville")
AND month = 7 AND day = 29 AND year = 2021
ORDER BY hour asc, minute asc
limit 1));


-- THEIF
select name from people where name in (select name from people, atm_transactions, bank_accounts
where atm_transactions.account_number = bank_accounts.account_number
and bank_accounts.person_id = people.id
and atm_location = "Leggett Street"
AND transaction_type = 'withdraw' AND month = 7 AND day = 28 AND year = 2021)
and name in (select name from people, bakery_security_logs
where bakery_security_logs.license_plate = people.license_plate
and month = 7 AND day = 28 AND year = 2021
AND hour = 10 AND minute >= 5 AND minute <=25
AND activity = "exit")
and name in (select name from people
where passport_number in (
select passport_number from passengers
where flight_id = (select flights.id from flights, airports
where airports.id = flights.origin_airport_id
and origin_airport_id = (select id from airports where city = "Fiftyville")
AND month = 7 AND day = 29 AND year = 2021
ORDER BY hour asc, minute asc
limit 1)))
and name in (select name from people, phone_calls
where people.phone_number = phone_calls.caller
and month = 7 AND day = 28 AND year = 2021
and duration <= 60);
-- the theif is Bruce

-- ACCOMPLICE
select phone_number from people where name = "Bruce";

select name from people, phone_calls
where people.phone_number = phone_calls.receiver
and month = 7 AND day = 28 AND year = 2021
and duration <= 60
and phone_calls.caller = (select phone_number from people where name = "Bruce");

-- The Accomplice is Robin

