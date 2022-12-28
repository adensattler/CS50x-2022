select avg(rating) from ratings
join movies on movies.id = ratings.movie_id
where movies.year = 2012;

--SELECT AVG(ALL rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);
