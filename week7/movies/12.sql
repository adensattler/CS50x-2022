select title from movies, stars, people
where movies.id = stars.movie_id
and stars.person_id = people.id
and name = "Johnny Depp"
and title in(
select title from movies, stars, people
where movies.id = stars.movie_id
and stars.person_id = people.id
and name = "Helena Bonham Carter");