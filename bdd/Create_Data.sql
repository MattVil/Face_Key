DELETE FROM DataAccount;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Tags;
DELETE FROM Sites;
DELETE FROM Users;

#Tags Insertion
INSERT INTO Tags (name_tag) VALUES ('Personnal');
INSERT INTO Tags (name_tag) VALUES ('Work');
INSERT INTO Tags (name_tag) VALUES ('Social Network');
INSERT INTO Tags (name_tag) VALUES ('Tool');
INSERT INTO Tags (name_tag) VALUES ('Mail');