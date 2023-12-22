
        SELECT m.title, r.rating
        FROM ratings r
        JOIN movies m ON r.movie_id = m.id
        WHERE m.year = 2010
        ORDER BY r.rating DESC, m.title;
