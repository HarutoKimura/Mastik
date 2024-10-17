FROM ubuntu:latest

# Update and install essential packages
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    binutils-dev \
    libdwarf-dev \
    libelf-dev \
    python3 \
    python3-pip

# Install Python packages for the GUI interface
RUN pip3 install wx numpy paramiko matplotlib tqdm

# Clone the Mastik repository
RUN git clone https://github.com/IAIK/mastik.git /mastik

# Set the working directory
WORKDIR /mastik

# Configure and build Mastik
RUN ./configure && make && make install

# # Set the default command
# CMD ["/bin/bash"]