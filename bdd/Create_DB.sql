DELETE FROM Data_app;
DELETE FROM Data_account;
DELETE FROM Settings;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Users;

DROP TABLE Data_app;
DROP TABLE Data_account;
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

CREATE TABLE Data_account(
	id_data SERIAL,
	id_account SERIAL REFERENCES Account (id_account),
	average_conn_time TIME,
	last_loc VARCHAR(100),
	average_freq_conn INT,
	current_fred_conn INT,
	last_con TIMESTAMP,
	mode_shedule_conn TIMESTAMP
);

CREATE TABLE Data_app(
	id_data_app SERIAL,
	id_user SERIAL REFERENCES Users(id_user),
	creation_date DATE,
	last_conn DATE,
	average_freq_conn INT,
	current_fred_conn INT
);
