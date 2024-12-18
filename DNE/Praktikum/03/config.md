# Konfigurationsbefehle für IPv6-Geräte

## **Router R1**

### **1. Basis-Konfiguration**
```plaintext
configure terminal
hostname R1
no ip domain-lookup
enable secret class
line console 0
password cisco
login
line vty 0 4
password cisco
login
exit
service password-encryption
write memory
```

### **2. IPv6-Adressierung**
```plaintext
ipv6 unicast-routing

interface g0/0/0
ipv6 address 2001:db8:acad:2::1/64
ipv6 address fe80::1 link-local
no shutdown

interface g0/0/1
ipv6 address 2001:db8:acad:1::1/64
ipv6 address fe80::1 link-local
no shutdown

interface loopback 1
ipv6 address 2111:abcd:40aa:33b4:3::1/64
ipv6 address fe80::1 link-local

interface loopback 2
ipv6 address 2111:abcd:40aa:33b4:4::1/64
ipv6 address fe80::1 link-local
```

### **3. Statisches Routing**
```plaintext
ipv6 route 2111:abcd:40aa:33b4:5::/64 2001:db8:acad:1::2
ipv6 route 2111:abcd:40aa:33b4:6::/64 2001:db8:acad:2::2
ipv6 route ::/0 2001:db8:acad:2::2
```

---

## **Router R2**

### **1. Basis-Konfiguration**
```plaintext
configure terminal
hostname R2
no ip domain-lookup
enable secret class
line console 0
password cisco
login
line vty 0 4
password cisco
login
exit
service password-encryption
write memory
```

### **2. IPv6-Adressierung**
```plaintext
ipv6 unicast-routing

interface g0/0/0
ipv6 address 2001:db8:acad:2::2/64
ipv6 address fe80::2 link-local
no shutdown

interface g0/0/1
ipv6 address 2001:db8:acad:1::2/64
ipv6 address fe80::2 link-local
no shutdown

interface loopback 1
ipv6 address 2111:abcd:40aa:33b4:5::1/64
ipv6 address fe80::2 link-local

interface loopback 2
ipv6 address 2111:abcd:40aa:33b4:6::1/64
ipv6 address fe80::2 link-local
```

### **3. Statisches Routing**
```plaintext
ipv6 route ::/0 2001:db8:acad:1::1
```

---

## **Switch S1**

### **1. Basis-Konfiguration**
```plaintext
configure terminal
hostname S1
no ip domain-lookup
enable secret class
line console 0
password cisco
login
line vty 0 4
password cisco
login
exit
service password-encryption

interface vlan 1
ipv6 address 2001:db8:acad:1::3/64
ipv6 address fe80::3 link-local
no shutdown

sdm prefer dual-ipv4-and-ipv6 default
reload
```

### **2. Nicht verwendete Schnittstellen deaktivieren**
```plaintext
interface range g1/0/1 - 24
shutdown
```

### **3. Speichern der Konfiguration**
```plaintext
write memory
```

---

## **Switch S2**

### **1. Basis-Konfiguration**
```plaintext
configure terminal
hostname S2
no ip domain-lookup
enable secret class
line console 0
password cisco
login
line vty 0 4
password cisco
login
exit
service password-encryption

interface vlan 1
ipv6 address 2001:db8:acad:2::3/64
ipv6 address fe80::4 link-local
no shutdown

sdm prefer dual-ipv4-and-ipv6 default
reload
```

### **2. Nicht verwendete Schnittstellen deaktivieren**
```plaintext
interface range g1/0/1 - 24
shutdown
```

### **3. Speichern der Konfiguration**
```plaintext
write memory
```

---

## **PC-Konfiguration**

### **1. IPv6-Adressierung**
```plaintext
IPv6-Adresse: 2001:db8:acad:1::100
Subnetzmaske: /64
Standardgateway: fe80::1
Link-Local-Adresse: fe80::3
```

---

**Hinweis:** Vergiss nicht, alle Konfigurationen mit `ping` und `show`-Befehlen zu überprüfen:
- `show ipv6 interface brief`
- `show ipv6 route`
- `ping` (für Konnektivitätstest).
