# Alarm
Alarm system FW for ambulance camper. Takes signals from door sensors and will begin alarming if activated while enabled.

## Development
### Environment Setup
Install west:
```
pip install west
```

<!--TODO: find a good way to lock to a version-->
Pull Zephyr wherever seems good:
```
west init <path-to-zephyr>
cd <path-to-zephyr>
west update
```

#### Build with Docker
Launch docker:
```
docker run -ti -v <path-to-this-project>:/workdir/alarm -v <path-to-zephyr>:/workdir/zephyr docker.io/zephyrprojectrtos/zephyr-build:latest
```

Then configure and build:
```
$ cd alarm
$ source /workdir/zephyr/zephyr/zephyr-env.sh
$ west build .
```

#### Build on Host
Eh, just use docker for now

