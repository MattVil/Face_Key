DELETE FROM DataAccount;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Tags;
DELETE FROM Sites;
DELETE FROM Users;

DROP TABLE DataAccount;
DROP TABLE SharedAccount;
DROP TABLE Account;
DROP TABLE Tags;
DROP TABLE Sites;
DROP TABLE Users;

CREATE TABLE Users(
	id_user SERIAL,
	name VARCHAR(30),
	first_name VARCHAR(30),
	gender BOOLEAN,
	pseudo VARCHAR(20),
	mail VARCHAR(75),
	password VARCHAR(100),
	language VARCHAR(30),
	creation_date DATE,
	last_conn_date DATE,
	average_freq_conn INT,
	current_fred_conn INT,
	CONSTRAINT user_pk PRIMARY KEY (id_user)
);

CREATE TABLE Sites(
	id_site SERIAL,
	domaine VARCHAR(30),
	login_input VARCHAR(30),
	password_input VARCHAR(30),
	nb_user_today INT,
	nb_user_week INT,
	nb_user_month INT,
	CONSTRAINT sites_pk PRIMARY KEY (id_site)
);

CREATE TABLE Tags(
	id_tag SERIAL,
	name_tag VARCHAR(30),
	CONSTRAINT tags_pk PRIMARY KEY (id_tag)
);

CREATE TABLE Account(
	id_account SERIAL,
	site SERIAL REFERENCES Sites (id_site),
	login VARCHAR(75),
	password VARCHAR(100),
	id_user SERIAL REFERENCES Users (id_user),
	tag SERIAL REFERENCES Tags (id_tag),
	CONSTRAINT account_pk PRIMARY KEY (id_account)
);

CREATE TABLE SharedAccount(
	id_sharedAccount SERIAL,
	expiration_date DATE,
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
