let now = new Date(); //it comes with current date/time

fetch('/date', {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "date": now.getDate, "hour": now.getHours })
})
    .then(response => response.json())
    .then(response => console.log(JSON.stringify(response)))