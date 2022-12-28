-- select distinct name from people, stars
-- where people.id = stars.person_id
-- and id in

--select person_id from stars where movie_id =

select name from people where id in (
select person_id from stars where movie_id in(
select movie_id from stars where person_id =
(select id from people where name = "Kevin Bacon" and birth = 1958)))
and name != "Kevin Bacon";