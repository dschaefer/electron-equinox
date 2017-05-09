if (process.argv.length < 3) {
    console.log('Missing build directory');
    process.exit(1);
}

const buildDir = process.argv[2];

const fs = require('fs');
const childProcess = require('child_process');

if (!fs.existsSync(buildDir)) {
    fs.mkdirSync(buildDir);
}

if (!fs.existsSync(buildDir + '/CMakeCache.txt')) {
    childProcess.spawnSync('cmake', ['..'], { cwd: buildDir, stdio: 'inherit'});
}

childProcess.spawnSync('cmake', ['--build', buildDir, '--target', 'install'], { stdio: 'inherit'});
