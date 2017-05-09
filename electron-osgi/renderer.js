const osgi = require('./lib/osgi');

osgi.sayHi().then((value) => {
    const p = document.createElement('p');
    p.innerText = value;
    document.body.appendChild(p);
});
