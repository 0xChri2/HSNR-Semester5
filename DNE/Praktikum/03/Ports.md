# Port- und IP-Zuweisungen

## **Router R1**
| Port            | IP-Adresse/Präfix           |
|-----------------|-----------------------------|
| G0/0/0          | 2001:db8:acad:2::1/64       |
|                 | fe80::1 (Link-Local)        |
| G0/0/1          | 2001:db8:acad:1::1/64       |
|                 | fe80::1 (Link-Local)        |
| Loopback 1      | 2111:abcd:40aa:33b4:3::1/64 |
|                 | fe80::1 (Link-Local)        |
| Loopback 2      | 2111:abcd:40aa:33b4:4::1/64 |
|                 | fe80::1 (Link-Local)        |

---

## **Router R2**
| Port            | IP-Adresse/Präfix           |
|-----------------|-----------------------------|
| G0/0/0          | 2001:db8:acad:2::2/64       |
|                 | fe80::2 (Link-Local)        |
| G0/0/1          | 2001:db8:acad:1::2/64       |
|                 | fe80::2 (Link-Local)        |
| Loopback 1      | 2111:abcd:40aa:33b4:5::1/64 |
|                 | fe80::2 (Link-Local)        |
| Loopback 2      | 2111:abcd:40aa:33b4:6::1/64 |
|                 | fe80::2 (Link-Local)        |

---

## **Switch S1**
| Port            | IP-Adresse/Präfix           | Angeschlossen an          |
|-----------------|-----------------------------|---------------------------|
| VLAN 1          | 2001:db8:acad:1::3/64       | PC (Ethernet)             |
|                 | fe80::3 (Link-Local)        |                           |
| G0/0/1          | -                           | Router R1 (G0/0/1)        |

---

## **Switch S2**
| Port            | IP-Adresse/Präfix           | Angeschlossen an          |
|-----------------|-----------------------------|---------------------------|
| VLAN 1          | 2001:db8:acad:2::3/64       | -                         |
|                 | fe80::4 (Link-Local)        |                           |
| G0/0/1          | -                           | Router R2 (G0/0/1)        |

---

## **PC**
| Schnittstelle   | IP-Adresse/Präfix           | Angeschlossen an          |
|-----------------|-----------------------------|---------------------------|
| Ethernet        | 2001:db8:acad:1::100/64     | Switch S1 (VLAN 1)        |
|                 | fe80::3 (Link-Local)        |                           |
| Standardgateway | fe80::1                     |                           |
