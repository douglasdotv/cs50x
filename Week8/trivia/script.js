const multipleChoiceAnswer = 'Brasília';
const freeResponseAnswers = ['Internet Protocol', 'Transmission Control Protocol', 'Hypertext Transfer Protocol'];

document.querySelectorAll('#multiple-choice button').forEach(button => {
    button.addEventListener('click', function() {
        const isCorrect = button.value == multipleChoiceAnswer
        updateButtonColor(isCorrect, button);
        updateAnswerStatus(isCorrect, 'multiple-choice');
    });
});

document.querySelector('#free-response div button').addEventListener('click', function() {
    const textArea = document.getElementById('free-response-answer');
    const userAnswer = textArea.value;
    const isCorrect = freeResponseAnswers.every(answer => userAnswer.includes(answer));
    updateTextAreaColor(isCorrect, textArea);
    updateAnswerStatus(isCorrect, 'free-response');
});

function updateButtonColor(isCorrect, button) {
    if (isCorrect) {
        button.style.backgroundColor = 'green';
    } else {
        button.style.backgroundColor = 'red';
    }
}

function updateTextAreaColor(isCorrect, textArea) {
    if (isCorrect) {
        textArea.style.backgroundColor = 'green';
    } else {
        textArea.style.backgroundColor = 'red';
    }
}

function updateAnswerStatus(isCorrect, questionType) {
    const answerStatusDiv = document.querySelector(`#${questionType} .answer-status`);
    if (isCorrect) {
        answerStatusDiv.textContent = 'Correct';
    } else if (answerStatusDiv.textContent !== 'Correct') {
        answerStatusDiv.textContent = 'Incorrect';
    }
}
