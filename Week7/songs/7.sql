
        SELECT AVG(s.energy)
        FROM songs s
        JOIN artists ON s.artist_id = artists.id
        WHERE artists.name = 'Drake';
