Masternode Instructions and Notes
=============================
 - Version 0.3.3
 - Date: 17 jan 2020

Prerequisites
-------------
 - Ubuntu 14.04+
 - Port **29500** is open
 - Libraries to build from BCZ source if you want to build it yourself

Step 0. ON VPS: Acquire the binaries
----------------------

Either

<details open>
<summary><strong>Download the prebuilt binaries</strong></summary>
<strong>0.1</strong> Install prebuild client and full chain

    wget https://github.com/SpecialCoins/bitcoincz/releases/download/6.0.3.2/linux-pc.tar.gz
    tar xvfz linux-pc.tar.gz
    ---
    (optional to remove old chain):
    cd .bcz
    rm -f -r blocks
    rm -f -r chainsate
	or/and:
	(option(2) to create chainfiles):
	mkdir .bcz
	cd .bcz
	sudo apt-get install unzip
	wget https://github.com/SpecialCoins/bitcoincz/releases/download/chainfiles/chainfiles.zip
	unzip chainfiles.zip
	cd ..
	---
	./bczd -daemon
	./bcz-cli getinfo

</details>

or

<details>
<summary><strong>Build from source</strong></summary>
<strong>0.1.</strong>  Check out from source:

    git clone https://github.com/SpecialCoins/bitcoincz

<strong>0.2.</strong>  See [README.md](README.md) for instructions on building.
</details>
	

Step 1. ON VPS: Open port 29500 (Optional - only if firewall is running)
----------------------
**1.1.**  Run:

    sudo ufw allow ssh 
    sudo ufw allow 29500
    sudo ufw default allow outgoing
    sudo ufw enable

Step 2. ON LOCAL MACHINE: First run on your Local Wallet
----------------------

<details open>
<summary><strong>If you are using the qt wallet</strong></summary>
<strong>2.0.</strong>  Open the wallet

<strong>2.1.</strong>  Click: Tools -> Debug window -> Console

<strong>2.2.</strong>  Generate masternodeprivkey:

    masternode genkey

(Store this key)

<strong>2.3.</strong>  Get wallet address:

    getaccountaddress MN1

<strong>2.4.</strong>  Send to received address <strong>exactly 5000 BCZ</strong> in <strong>1 transaction</strong>. Wait for 15 confirmations.

<strong>2.5.</strong>  Close the wallet
</details>

<details>
<summary><strong>If you are using the daemon</strong></summary>
<strong>2.0.</strong>  Go to the checked out folder or where you extracted the binaries

    cd BCZ/src

<strong>2.1.</strong>  Start daemon:

    ./bczd -daemon -server

<strong>2.2.</strong>  Generate masternodeprivkey:

    ./bcz-cli masternode genkey

(Store this key)

<strong>2.3.</strong>  Get wallet address:

    ./bcz-cli getaccountaddress MN1

<strong>2.4.</strong>  Send to received address <strong>exactly 5000 BCZ</strong> in <strong>1 transaction</strong>. Wait for 15 confirmations.

<strong>2.5.</strong>  Stop daemon:

    ./bcz-cli stop
</details>


## For both:

**2.6.**  Create file **masternode.conf** (in **~/.BCZ**, **C:\Users\USER\AppData\Roaming\BCZ** or **~/Library/Application Support/BCZ** depending on your Operating System) containing the following info:
 - LABEL: A one word name you make up to call your node (ex. XN1)
 - IP:PORT: Your masternode VPS's IP, and the port is always 29500.
 - MASTERNODEPRIVKEY: This is the result of your "masternode genkey" from earlier.
 - TRANSACTION HASH: The collateral tx. hash from the 5000 BCZ deposit.
 - INDEX: The Index from the transaction hash

To get TRANSACTION HASH, run:

```
./bcz-cli masternode outputs
```
or
```
masternode outputs
```

depending on your wallet/daemon setup.

The output will look like:

    { "d6fd38868bb8f9958e34d5155437d009b72dfd33fc28874c87fd42e51c0f74fdb" : "0", }

Sample of masternode.conf:

    MN1 51.52.53.54:29100 XrxSr3fXpX3dZcU7CoiFuFWqeHYw83r28btCFfIHqf6zkMp1PZ4 d6fd38868bb8f9958e34d5155437d009b72dfd33fc28874c87fd42e51c0f74fdb 0

**2.7.** Lock unspent

As long as the masternode is listed in your masternode.conf file the funds are automatically locked so you don't accidentially spend them.

Step 3. ON VPS: Update config files
----------------------
**3.1.**  Create file **bcz.conf** (in folder **~/.BCZ**)

    server=1
    masternode=1
    masternodeprivkey=XXXXXXXXXXXXXXXXX  ## Replace with your masternode private key
    externalip=XXX.XXX.XXX.XXX ## Replace with your node external IP


Step 4. ON LOCAL MACHINE: Start the masternode
----------------------

<details open>
<summary><strong>With qt wallet</strong></summary>
<strong>4.1</strong> Start the masternode via your gui wallet in the masternodes tab
</details>

<details>
<summary><strong>With daemon</strong></summary>
<strong>4.1</strong> Start masternode:

    ./bcz-cli masternode start-alias <LABEL>

For example:

    ./bcz-cli masternode start-alias MN1

<strong>4.2</strong>  To check node status:

    ./bcz-cli masternode debug

</details>


If not successfully started, just repeat start command
