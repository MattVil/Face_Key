DELETE FROM Settings;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Users;

DROP TABLE Settings;
DROP TABLE SharedAccount;
DROP TABLE Account;
DROP TABLE Users;

CREATE TABLE Users(
	id_user SERIAL,
	pseudo VARCHAR(20),
	mail_adr VARCHAR(75),
	password VARCHAR(100),
	CONSTRAINT user_pk PRIMARY KEY (id_user)
);

CREATE TABLE Account(
	id_account SERIAL,
	site VARCHAR(100),
	login VARCHAR(75),
	password VARCHAR(100),
	id_user SERIAL REFERENCES Users (id_user),
	CONSTRAINT account_pk PRIMARY KEY (id_account)
);

CREATE TABLE SharedAccount(
	id_share SERIAL,
	expire_date DATE,
	id_account SERIAL REFERENCES Account (id_account),
	id_user SERIAL REFERENCES Users (id_user),
	CONSTRAINT sharedaccount_pk PRIMARY KEY (id_share)
);

CREATE TABLE Settings(
	id_user SERIAL REFERENCES Users (id_user),
	language VARCHAR(20),
	CONSTRAINT settings_pk PRIMARY KEY (id_user)
);