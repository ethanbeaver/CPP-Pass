CPP-Pass
========

The information you had us share with the other team is located in `README.md`. Below is the information you specifically asked for.

Admission of Fault
------------------

This section is intended only for the professor.

Ok, so there is actually one vulnerability, and we blame Ethan and Andy for this one. Plain text strings are never cleared, which means they can be snooped on either by segfault core dump or memory dump from root.

While memory dumps are a concern, the user needs the key file to open the safe in the first place. It's an attack that must be carried out while having already opened the safe. As a result, we don't see this attack vector as a big issue.

But if you really want to get picky, there is one tiny security issue. AES requires both a key and an initialization vector, or IV. The IV randomizes the first block. To brute force AES-256 with an IV, you'll need to check approximately 2^(256+128-1) times (or about 10^115 times) for a 50% chance of breaking it. Even the right key with the wrong IV won't decrypt the cipher. Our implementation sets the IV to 0, which reduces the complexity to 2^(256-1). In our opinion, this is still prohibitively large for an undergrad class.

To make the safe harder to brute force, we could simply randomize the IV and tack it onto the front of the encrypted file. Any sort of rainbow tables would become useless.

Other attack vectors are not within the scope of this project, such as keyloggers and screen captures. It is very difficult to defend against these kinds of attacks at the userland level. Once a user has access to the machine itself, it's much tougher to protect against intrusion.

Broadly speaking, this program is quite sturdy. Until an error in AES-256 is found or released, we can safely assume this key file cannot be broken in a matter of days or weeks, or probably even years, or maybe even millennia.

All secure backend code has been tested using unit tests. While no unit test set is ever 100% complete, it ensures data going in and coming out is compatible and can at least pretend to be safe. Analysis of the password safe file shows that passwords are not human-readable.

However, if an attacker were to somehow break in, they would have access to all passwords stored in the safe. Passwords could include bank logins, work accounts, or credit card information.

Contributions
-------------

**Andy Russell:**

**Ethan Beaver:**

**Quillan Jacobson:**
