document.addEventListener("DOMContentLoaded", function() {
    const audioFiles = [
        'https://ia902305.us.archive.org/2/items/news-at-11-wmy7d8/%E7%8C%AB%20%E3%82%B7%20Corp.%20-%20NEWS%20AT%2011%20-%2020%20THE%20WEATHER%20CHANNEL%2011.mp3',
        'https://ia802609.us.archive.org/16/items/w-w-w-d-e-e-p-d-i-v-e-c-o-m-wg7bkk/Webinar%E2%84%A2%20-%20w%20w%20w%20.%20d%20e%20e%20p%20d%20i%20v%20e%20.%20c%20o%20m%20-%2002%20%E6%B3%A2.mp3',
        'https://ia802904.us.archive.org/15/items/onehundredmornings/Windows96%20-%20One%20Hundred%20Mornings%20-%2003%20Abstract.mp3'
    ];

    const audioPlayer = document.getElementById('audio-player');
    const audioSource = document.getElementById('audio-source');
    const playButton = document.getElementById('play-button');

    function setRandomAudioSource() {
        const randomIndex = Math.floor(Math.random() * audioFiles.length);
        const randomAudioFile = audioFiles[randomIndex];

        audioSource.src = randomAudioFile;
        audioPlayer.load();
    }

    playButton.addEventListener('click', function() {
        setRandomAudioSource();
        audioPlayer.play();
    });
});
