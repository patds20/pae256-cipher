<h1 style="color:blue;">PAE256 - A Simple Symmetric Cipher</h1>
	<hr>
	<h2 style="color:purple;">Introduction</h2>
	<p>PAE256 is a simple yet effective symmetric cipher that can be used for encrypting data. It works by taking a plaintext and a key (256 Byte) as input and outputting a ciphertext. The ciphertext can then be decrypted back to the plaintext using the same key. The algorithm is named after the steps it performs: <b>P</b>ermute and <b>A</b>dding (XOR) - (<b>E</b>ncryption). </p>
	<h2 style="color:purple;">How it works</h2>
	<p>The PAE256 algorithm works by performing a series of operations on the plaintext and the key to produce the ciphertext. The basic steps of the algorithm are:</p>
	<ul>
		<li>Permute the plaintext matrix</li>
		<li>Perform XOR of cells in plaintext matrix (based on indices key values)</li>
		<li>Invert single cells (based on indices key values)</li>
                <li>Permute the plaintext matrix again</li>
		<li>XOR the plaintext matrix with the key matrix</li>
		<li>Repeat the above steps and change the key</li>
		<li>Generate new round key for next chunk of 256 Byte</li>
	</ul>
	<p>Each step in the algorithm is designed to scramble the input data in a way that is difficult to reverse without the key. The use of XOR and permutation ensures that even small changes in the input data will produce a completely different output. </p>
	<h2 style="color:purple;">Usage</h2>
	<p>To use PAE256, you need to have a plaintext file and a key file. The plaintext file should contain the data you want to encrypt, and the key file should contain a random sequence of bytes that is 256 bytes long. You can then run the pae256 program to encrypt the plaintext file using the key file:</p>
	<pre>pae256 -ek [path to plaintext] [path to key] [path to output]</pre>
	<p>The script will output the encrypted data to the [path to output file. To decrypt the ciphertext, you simply need to run the script again with the ciphertext file and the same key file:</p>
	<pre>pae256 -d [path to ciphertext] [path to key] [path to output]</pre>
	<h2 style="color:purple;">Conclusion</h2>
	<p>PAE256 is a simple yet effective symmetric cipher that can be used to encrypt data. It is easy to implement and can provide a reasonable level of security for most use cases. However, like any encryption algorithm, it is not perfect and should not be relied upon as the sole means of protecting sensitive data. Nevertheless, it is a useful example of how symmetric encryption works and can be a good starting point for anyone interested in learning more about cryptography.</p>

  
