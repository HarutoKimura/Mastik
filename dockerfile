FROM --platform=linux/amd64 ubuntu:20.04 

# Update and install essential packages, binutils-dev libdwarf-dev libelf-dev are default dependencies of Mastik and libgmp-dev is required for the GMP library
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    binutils-dev \
    libdwarf-dev \
    libelf-dev \
    libgmp-dev \ 
    python3 \
    python3-pip
# Install Python packages to plot data collected by Mastik
RUN pip3 install pandas matplotlib
# Clone the Mastik repository and checkout the feature/flush_reload branch
RUN git clone --branch feature/flush_reload https://github.com/HarutoKimura/mastik.git /mastik

# Set the working directory
WORKDIR /mastik/demo/group13_flush_and_reload_experiment

# Configure and build Mastik
RUN ./configure && make && make install

# # Set the default command
# CMD ["/bin/bash"]