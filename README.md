# Sistemas Operativos: TP2

Basado en [TP_ARQUI_20212Q](https://github.com/Pedro-UwU/TP_ARQUI_20212Q) de 
López Guzmán, Pedro Jeremías

## Build

### Environment

In order to build and run this project, you'll need to use a special Docker
container.
There's a Bash script made to automate this process.

Grant execution permissions to `build_docker_image.sh` and run it.
```bash
chmod +x env/build_docker_image.sh
./env/build_docker_image.sh
```

### Source code

Grant execution permissions to `run_docker.sh` script and start the previously
builded docker image.
```bash
chmod +x env/run_docker.sh
./env/run_docker.sh
```

Once inside the container, build the project source code using make
```bash
make
```

If you want to use a Buddy memory manager (instead of the default based on K&R)
compile as
```bash
make buddy
```

### Testing

There are some tests written for this project and they can be run 
(inside the Docker container) as:
```bash
make test
```

And for testing the Buddy memory manager
```bash
make buddyTest
```

## License

This project and all its files are licensed under the BSD 3-Clause "New" or
"Revised" License

> Copyright (c) 2022 Flores Levalle, M.,
>                    López, P.,
>                    Sierra Pérez, C.,
>                    Zahnd, M. E.,
> 

See [LICENSE](LICENSE) for details.
