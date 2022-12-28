SELECT DISTINCT name from people, directors, ratings
where people.id = directors.person_id
and directors.movie_id = ratings.movie_id
and rating >= 9.0;