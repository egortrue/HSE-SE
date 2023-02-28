import random
import argparse


def handle_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('particles_count', action='store',
                        type=int, help='Number of particles')
    parser.add_argument('steps', action='store',
                        type=int, help='Number of steps')
    parser.add_argument('filename', action='store', type=str,
                        help='Destination of generation')

    parser.add_argument(
        '-mmin', type=float, help="Minimum mass for body (default=10000.0)", default=10000.0)
    parser.add_argument(
        '-mmax', type=float, help="Maximum mass for body (default=50000.0)", default=50000.0)
    parser.add_argument(
        '-xmin', type=float, help="Minimum x position for body (default=-10.0)", default=-10.0)
    parser.add_argument(
        '-xmax', type=float, help="Maximum x position for body (default=10.0)", default=10.0)
    parser.add_argument(
        '-ymin', type=float, help="Minimum y position for body (default=-10.0)", default=-10.0)
    parser.add_argument(
        '-ymax', type=float, help="Maximum y position for body (default=10.0)", default=10.0)
    parser.add_argument(
        '-vxmin', type=float, help="Minimum speed projection on x for body (default=-5.0)", default=-5.0)
    parser.add_argument(
        '-vxmax', type=float, help="Maximum speed projection on x for body (default=5.0)", default=5.0)
    parser.add_argument(
        '-vymin', type=float, help="Minimum speed projection on y for body (default=-5.0)", default=-5.0)
    parser.add_argument(
        '-vymax', type=float, help="Maximum speed projection on y for body (default=5.0)", default=5.0)

    return parser.parse_args()


def generate(args):
    open(args.filename, 'w').close()
    with open(args.filename, 'a') as f:
        f.write(f'6.6743e-11 {args.particles_count} {args.steps}\n')
        for _ in range(0, args.particles_count):
            mass = random.uniform(args.mmin, args.mmax)
            x = random.uniform(args.xmin, args.xmax)
            y = random.uniform(args.ymin, args.ymax)
            vx = random.uniform(args.vxmin, args.vxmax)
            vy = random.uniform(args.vymin, args.vymax)

            f.write(f'{mass}\n{x} {y}\n{vx} {vy}\n')


if __name__ == '__main__':
    args = handle_args()
    generate(args)
