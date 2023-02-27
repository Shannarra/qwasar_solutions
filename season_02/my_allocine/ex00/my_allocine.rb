requests['Display all actors'] = "SELECT * FROM actors;"
requests['Display all genres'] = "SELECT * from genres;"
requests['Display the name and year of the movies'] = "select mov_title, mov_year from movies;"
requests['Display reviewer name from the reviews table'] = "select rev_name from reviews;"

requests["Find the year when the movie American Beauty released"] = "select mov_year from movies where mov_title='American Beauty';"
requests["Find the movie which was released in the year 1999"] = "select mov_title from movies where mov_year=1999;"
requests["Find the movie which was released before 1998"] = "select mov_title from movies where mov_year<1998;"
requests["Find the name of all reviewers who have rated 7 or more stars to their rating order by reviews rev_name (it might have duplicated names :-))"] = "SELECT rev_name from reviews INNER JOIN movies_ratings_reviews on movies_ratings_reviews.rev_id = reviews.id where rev_stars >= 7 ORDER BY rev_name;"
requests["Find the titles of the movies with ID 905, 907, 917"] = "SELECT mov_title from movies where id in (905, 907, 917);"
requests["Find the list of those movies with year and ID which include the words Boogie Nights"] = "SELECT id, mov_title, mov_year from movies where mov_title like '%Boogie Nights%';"
requests["Find the ID number for the actor whose first name is 'Woody' and the last name is 'Allen'"] = "select id FROM actors where act_fname='Woody';"

# mov_id = 911
requests["Find the actors with all information who played a role in the movies 'Annie Hall'"] = "select * from actors where id=111;"
requests["Find the first and last names of all the actors who were cast in the movies 'Annie Hall', and the roles they played in that production"] = "select act_fname, act_lname, role from actors JOIN movies_actors on movies_actors.act_id=id where mov_id=911;"
# mov_id = 916
requests["Find the name of movie and director who directed a movies that casted a role as Sean Maguire"] = "select dir_fname, dir_lname, mov_title from directors JOIN movies on movies.id=916 where directors.id=(select dir_id from directors_movies where mov_id=916);"
requests["Find all the actors who have not acted in any movie between 1990 and 2000 (select only actor first name, last name, movie title and release year)"] = "SELECT act_fname, act_lname, mov_title, mov_year from actors join movies_actors on actors.id=movies_actors.act_id join movies on movies.id=mov_id where mov_year>=2000 or mov_year<=1990;"