select title from movies, stars, people, ratings
where movies.id = stars.movie_id
and stars.person_id = people.id
and movies.id = ratings.movie_id
and name = "Chadwick Boseman" ORDER BY rating desc limit 5;