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
INSERT INTO Tags (name_tag) VALUES ('News');
INSERT INTO Tags (name_tag) VALUES ('Shop');
INSERT INTO Tags (name_tag) VALUES ('Music');

#Sites Insertion
INSERT INTO Sites (domain) VALUES ('facebook.com');
INSERT INTO Sites (domain) VALUES ('twitter.com');
INSERT INTO Sites (domain) VALUES ('google.com');
INSERT INTO Sites (domain) VALUES ('trello.com');
INSERT INTO Sites (domain) VALUES ('evernote.com');
INSERT INTO Sites (domain) VALUES ('nytimes.com');
INSERT INTO Sites (domain) VALUES ('amazon.com');
INSERT INTO Sites (domain) VALUES ('spotify.com');