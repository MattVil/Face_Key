DELETE FROM PaymentAccount;
DELETE FROM DataAccount;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Tags;
DELETE FROM Sites;
DELETE FROM Users;

DROP TABLE PaymentAccount;
DROP TABLE DataAccount;
DROP TABLE SharedAccount;
DROP TABLE Account;
DROP TABLE Tags;
DROP TABLE Sites;
DROP TABLE Users;

CREATE TABLE Users(
	id_user SERIAL,
	name VARCHAR(30) NOT NULL,
	first_name VARCHAR(30) NOT NULL,
	gender BOOLEAN NOT NULL,
	pseudo VARCHAR(20) NOT NULL,
	mail VARCHAR(75) NOT NULL,
	password VARCHAR(100) NOT NULL,
	language VARCHAR(30) NOT NULL,
	creation_date DATE NOT NULL,
	last_conn_date DATE,
	average_freq_conn INT,
	current_fred_conn INT,
	CONSTRAINT user_pk PRIMARY KEY (id_user)
);

CREATE TABLE Sites(
	id_site SERIAL,
	domain VARCHAR(50) NOT NULL,
	login_input VARCHAR(30),
	password_input VARCHAR(30),
	nb_user_today INT,
	nb_user_week INT,
	nb_user_month INT,
	CONSTRAINT sites_pk PRIMARY KEY (id_site)
);

CREATE TABLE Tags(
	id_tag SERIAL,
	name_tag VARCHAR(30) NOT NULL,
	CONSTRAINT tags_pk PRIMARY KEY (id_tag)
);

CREATE TABLE Account(
	id_account SERIAL,
	id_site SERIAL REFERENCES Sites (id_site),
	login VARCHAR(75) NOT NULL,
	password VARCHAR(100) NOT NULL,
	id_user SERIAL REFERENCES Users (id_user),
	id_tag SERIAL REFERENCES Tags (id_tag),
	CONSTRAINT account_pk PRIMARY KEY (id_account)
);

CREATE TABLE SharedAccount(
	id_sharedAccount SERIAL,
	expiration_date DATE NOT NULL,
	id_account SERIAL REFERENCES Account (id_account),
	id_receiver SERIAL REFERENCES Users (id_user),
	CONSTRAINT sharedaccount_pk PRIMARY KEY (id_sharedAccount)
);

CREATE TABLE DataAccount(
	id_data SERIAL,
	id_account SERIAL REFERENCES Account (id_account),
	average_conn_time TIME,
	last_loc VARCHAR(100),
	average_freq_conn INT,
	last_con TIMESTAMP,
	link_to_freq VARCHAR(30),
	link_to_time VARCHAR(30),
	CONSTRAINT dataAccount_pk PRIMARY KEY (id_data)
);

CREATE TABLE PaymentAccount(
	bank VARCHAR(50),
	RIB VARCHAR(30),
	card_num VARCHAR(16),
	cryptogram VARCHAR(3)
);
