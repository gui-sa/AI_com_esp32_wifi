var delayInMilliseconds = 1000; //1 second

setTimeout(function () {
    //your code to be executed after 1 second
}, delayInMilliseconds);

let now = new Date(); //it comes with current date/time

fetch('/date', {
    method: 'POST',
    headers: {
        'Content-Type': 'text/plain'
    },
    body: "D" + now.getDay() + "M" + now.getMonth() + "Y" + now.getFullYear() + "H" + now.getHours() + "N" + now.getMinutes() + "S" + now.getSeconds()
})
    .then(response => console.log(response))