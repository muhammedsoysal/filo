# 🍽️ 42 Philosophers - Complete Guide

## 📚 Table of Contents

1. [Proje Genel Bakış](#proje-genel-bakış)
2. [Temel Kavramlar](#temel-kavramlar)
3. [Thread Nedir?](#thread-nedir)
4. [Mutex Nedir?](#mutex-nedir)
5. [Monitor Nedir?](#monitor-nedir)
6. [Proje Mimarisi](#proje-mimarisi)
7. [Çatal Sistemi](#çatal-sistemi)
8. [Filozof Durumları](#filozof-durumları)
9. [Senaryo Analizleri](#senaryo-analizleri)
10. [Kod Detayları](#kod-detayları)
11. [Test Senaryoları](#test-senaryoları)
12. [Sık Sorulan Sorular](#sık-sorulan-sorular)

---

## 🎯 Proje Genel Bakış

**42 Philosophers**, klasik "Dining Philosophers Problem" (Filozofların Yemek Problemi) üzerine kurulu bir multithreading projesidir.

### 🎭 Problem Tanımı

- **N filozof** yuvarlak bir masada oturuyor
- Her filozofun **solunda ve sağında bir çatal** var
- Filozoflar **yemek yemek**, **uyumak** ve **düşünmek** istiyor
- **Yemek yemek için 2 çatal gerekli** (sol + sağ)
- **Deadlock** ve **açlık** önlenmeli

### 🎪 Proje Kuralları

1. Her filozof **2 çatal** almalı (yemek için)
2. **Deadlock** olmamalı
3. **Açlık** olmamalı
4. Filozoflar **belirli sürelerde** işlem yapmalı
5. **Ölüm kontrolü** yapılmalı

---

## 🔧 Temel Kavramlar

### 🧵 Thread (İş Parçacığı)

- **Thread**, bir programın **paralel çalışan** en küçük birimidir
- **Aynı process** içinde birden fazla thread olabilir
- **Paylaşılan bellek** alanına erişebilir
- **Hafif** ve **hızlı** context switching

### 🔒 Mutex (Mutual Exclusion)

- **Mutex**, aynı anda **sadece bir thread**'in erişebileceği kaynakları korur
- **Lock/Unlock** mekanizması
- **Race condition**'ları önler
- **Kritik bölümleri** korur

### 👁️ Monitor

- **Monitor**, birden fazla thread'i **senkronize** eden yapı
- **Durum kontrolü** yapar
- **Thread'leri bekletir** veya **uyandırır**
- **Koşullu senkronizasyon** sağlar

---

## 🧵 Thread Nedir?

### 📖 Tanım

**Thread**, bir programın **paralel çalışan** en küçük birimidir. Ana program (process) içinde birden fazla thread aynı anda çalışabilir.

### 🔄 Thread vs Process

| Özellik            | Process           | Thread            |
| ------------------ | ----------------- | ----------------- |
| **Bellek**         | Ayrı bellek alanı | Paylaşılan bellek |
| **Oluşturma**      | Yavaş             | Hızlı             |
| **Context Switch** | Yavaş             | Hızlı             |
| **İletişim**       | IPC gerekli       | Doğrudan          |
| **Kaynak**         | Çok               | Az                |

### 🎭 Thread Yaşam Döngüsü

```
[Yeni] → [Çalışıyor] → [Bekliyor] → [Çalışıyor] → [Bitti]
   ↓         ↓           ↓           ↓         ↓
pthread_create()  mutex_lock()  mutex_unlock()  pthread_join()
```

### 💻 C'de Thread Kullanımı

```c
#include <pthread.h>

// Thread fonksiyonu
void* thread_function(void* arg) {
    // Thread işlemleri
    return NULL;
}

int main() {
    pthread_t thread_id;

    // Thread oluştur
    pthread_create(&thread_id, NULL, thread_function, NULL);

    // Thread'i bekle
    pthread_join(thread_id, NULL);

    return 0;
}
```

---

## 🔒 Mutex Nedir?

### 📖 Tanım

**Mutex** (Mutual Exclusion), aynı anda **sadece bir thread**'in erişebileceği kaynakları koruyan senkronizasyon mekanizmasıdır.

### 🔐 Mutex Çalışma Prensibi

```
Thread A: mutex_lock() → [Kritik Bölüm] → mutex_unlock()
Thread B: mutex_lock() → [Bekliyor...] → [Kritik Bölüm] → mutex_unlock()
```

### ⚠️ Mutex Olmadan (Race Condition)

```c
// TEHLİKELİ - Race Condition
int counter = 0;

void* increment(void* arg) {
    for(int i = 0; i < 1000; i++) {
        counter++; // Aynı anda birden fazla thread erişebilir!
    }
    return NULL;
}
```

### ✅ Mutex ile (Güvenli)

```c
// GÜVENLİ - Mutex korumalı
int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* arg) {
    for(int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);
        counter++; // Sadece bir thread erişebilir
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
```

### 🔑 Mutex Türleri

1. **Normal Mutex**: Standart mutex
2. **Recursive Mutex**: Aynı thread birden fazla lock yapabilir
3. **Error Checking Mutex**: Hata kontrolü yapar
4. **Adaptive Mutex**: Performans optimizasyonu

---

## 👁️ Monitor Nedir?

### 📖 Tanım

**Monitor**, birden fazla thread'i **senkronize** eden ve **koşullu bekletme** sağlayan yapıdır.

### 🎭 Monitor Bileşenleri

1. **Mutex**: Kritik bölümleri korur
2. **Condition Variables**: Koşullu bekletme
3. **Shared Data**: Paylaşılan veriler

### 🔄 Monitor Çalışma Prensibi

```c
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int data;
} Monitor;

void monitor_wait(Monitor* m) {
    pthread_mutex_lock(&m->mutex);
    while (!condition_met) {
        pthread_cond_wait(&m->condition, &m->mutex);
    }
    // İşlem yap
    pthread_mutex_unlock(&m->mutex);
}

void monitor_signal(Monitor* m) {
    pthread_mutex_lock(&m->mutex);
    // Koşulu değiştir
    pthread_cond_signal(&m->condition);
    pthread_mutex_unlock(&m->mutex);
}
```

### 🎪 42 Philosophers'da Monitor

Projede **monitor thread** kullanılarak:

- **Ölüm kontrolü** yapılıyor
- **Yemek sayısı** takip ediliyor
- **Program sonlandırma** kontrol ediliyor

---

## 🏗️ Proje Mimarisi

### 📁 Dosya Yapısı

```
philo/
├── philo.h          # Header dosyası (struct tanımları)
├── main.c           # Ana program (thread oluşturma)
├── routine.c        # Filozof ve monitor rutinleri
├── utils.c          # Yardımcı fonksiyonlar
└── Makefile         # Derleme dosyası
```

### 🧩 Struct Yapıları

#### 📋 t_args (Argümanlar)

```c
typedef struct s_args
{
    int n_philo;                    // Toplam filozof sayısı
    int time_to_die;                // Ölüm süresi (ms)
    int time_to_eat;                // Yemek süresi (ms)
    int time_to_sleep;              // Uyku süresi (ms)
    int n_must_eat;                 // Minimum yemek sayısı
    long start_time;                // Program başlangıç zamanı
    pthread_mutex_t *forks;         // Çatal mutex'leri
    pthread_mutex_t print_mutex;    // Yazdırma mutex'i
    pthread_mutex_t death_mutex;    // Ölüm kontrol mutex'i
    int one_died;                   // Ölüm durumu
    int finished_eating;            // Yemek bitiren sayısı
} t_args;
```

#### 👤 t_philo (Filozof)

```c
typedef struct s_philo
{
    int id;                         // Filozof ID'si
    int ate_count;                  // Yediği yemek sayısı
    long last_meal;                 // Son yemek zamanı
    pthread_t thread;               // Filozof thread'i
    pthread_mutex_t *left_fork;     // Sol çatal
    pthread_mutex_t *right_fork;    // Sağ çatal
    t_args *args;                   // Argümanlar
} t_philo;
```

### 🔄 Program Akışı

```
1. Argümanları parse et
2. Mutex'leri başlat
3. Filozof struct'larını oluştur
4. Filozof thread'lerini oluştur
5. Monitor thread'ini oluştur
6. Thread'leri bekle
7. Kaynakları temizle
```

---

## 🍴 Çatal Sistemi

### 🎯 Çatal Dağılımı

```
        Çatal 1
    Filozof 1
        Çatal 2
    Filozof 2
        Çatal 3
    Filozof 3
        Çatal 4
    Filozof 4
        Çatal 5
    Filozof 5
        Çatal 1 (döngüsel)
```

### 🔐 Çatal Mutex Sistemi

```c
// Çatal mutex'leri
args.forks = malloc(sizeof(pthread_mutex_t) * args.n_philo);

// Her filozof için çatal ataması
philos[i].left_fork = &args.forks[i];
philos[i].right_fork = &args.forks[(i + 1) % args.n_philo];
```

### ⚠️ Deadlock Önleme

```c
// Çift ID'li filozoflar: önce sol, sonra sağ
if (philo->id % 2 == 0) {
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
}
// Tek ID'li filozoflar: önce sağ, sonra sol
else {
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(philo->left_fork);
}
```

---

## 🎭 Filozof Durumları

### 🔄 Durum Makinesi

```
[Başlangıç] → [Çatal Alma] → [Yemek] → [Uyku] → [Düşünme] → [Çatal Alma]
     ↓              ↓           ↓        ↓         ↓           ↓
  Thread         take_forks()  eat()   sleep()   think()   take_forks()
  oluştur
```

### 📊 Durum Tablosu

| Durum          | Açıklama             | Süre          |
| -------------- | -------------------- | ------------- |
| **Çatal Alma** | Sol ve sağ çatalı al | Anlık         |
| **Yemek**      | Yemek yeme           | time_to_eat   |
| **Uyku**       | Uyuma                | time_to_sleep |
| **Düşünme**    | Düşünme              | Anlık         |

### ⏰ Zamanlama

```c
// Yemek yeme
ft_usleep(philo->args->time_to_eat);

// Uyku
ft_usleep(philo->args->time_to_sleep);

// Ölüm kontrolü
if (get_time() - philos[i].last_meal > args->time_to_die) {
    // Ölüm!
}
```

---

## 🎪 Senaryo Analizleri

### 🎭 Tek Filozof Senaryosu

```bash
./philo 1 800 200 200
```

#### 📊 Ne Olur?

1. **Filozof 1** çatal 1'i alır
2. **Çatal 2 yok** (sadece 1 filozof olduğu için)
3. **Yemek yiyemez** (2 çatal gerekli)
4. **Ölür** (800ms sonra)

#### 🔍 Kod Analizi

```c
// 1 filozof olduğunda özel durum
if (philo->args->n_philo == 1)
{
    pthread_mutex_lock(philo->left_fork);
    print_action(philo, "has taken a fork");
    pthread_mutex_unlock(philo->left_fork);
    return; // Yemek yemez, sadece çatalı alır
}
```

### 🎭 Çift Filozof Senaryosu

```bash
./philo 2 800 200 200
```

#### 📊 Ne Olur?

1. **Filozof 1**: Çatal 1 + 2 alır → Yemek yer
2. **Filozof 2**: Çatal 2 + 1 alır → Yemek yer
3. **Deadlock olmaz** (farklı sırada çatal alırlar)

#### 🔍 Kod Analizi

```c
// Çift ID'li filozoflar: önce sol, sonra sağ
if (philo->id % 2 == 0) {
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
}
// Tek ID'li filozoflar: önce sağ, sonra sol
else {
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(philo->left_fork);
}
```

### 🎭 Çoklu Filozof Senaryosu

```bash
./philo 4 310 200 100
```

#### 📊 Ne Olur?

1. **İlk tur**: Filozof 1 ve 3 yemek yer
2. **İkinci tur**: Filozof 2 ve 4 yemek yer
3. **Üçüncü tur**: Filozof 1 ve 3 düşünür
4. **Ölüm**: Filozof 1 ölür (310ms sonra yemek yiyemedi)

---

## 💻 Kod Detayları

### 🔑 Ana Fonksiyonlar

#### 🎭 philo_routine()

```c
void *philo_routine(void *philo)
{
    t_philo *p = (t_philo *)philo;

    // Çift ID'li filozoflar biraz bekler
    if (p->id % 2 == 0)
        ft_usleep(p->args->time_to_eat / 2);

    while (!is_dead(p)) {
        eat(p);                    // Yemek yeme

        // Yeterince yediyse çık
        if (p->args->n_must_eat != -1 &&
            p->ate_count >= p->args->n_must_eat) {
            p->args->finished_eating++;
            break;
        }

        sleep_and_think(p);        // Uyku ve düşünme
    }
    return (NULL);
}
```

#### 🍴 eat()

```c
void eat(t_philo *philo)
{
    take_forks(philo);            // Çatalları al

    // 1 filozof olduğunda yemek yiyemez
    if (philo->args->n_philo == 1)
        return;

    // Yemek yeme işlemi
    pthread_mutex_lock(&philo->args->death_mutex);
    philo->last_meal = get_time();
    philo->ate_count++;
    pthread_mutex_unlock(&philo->args->death_mutex);

    print_action(philo, "is eating");
    ft_usleep(philo->args->time_to_eat);

    // Çatalları bırak
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}
```

#### 👁️ monitor_routine()

```c
void *monitor_routine(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_args *args = philos[0].args;

    while (1) {
        for (int i = 0; i < args->n_philo; i++) {
            pthread_mutex_lock(&args->death_mutex);

            // Ölüm kontrolü
            if (get_time() - philos[i].last_meal > args->time_to_die) {
                args->one_died = 1;
                pthread_mutex_unlock(&args->death_mutex);
                printf("%ld %d died\n",
                       get_time() - args->start_time, philos[i].id);
                return (NULL);
            }

            // Herkes yeterince yediyse bitir
            if (args->n_must_eat != -1 &&
                args->finished_eating >= args->n_philo) {
                args->one_died = 1;
                pthread_mutex_unlock(&args->death_mutex);
                printf("all philo ate enough\n");
                return (NULL);
            }

            pthread_mutex_unlock(&args->death_mutex);
        }
        usleep(1000); // 1ms bekle
    }
    return (NULL);
}
```

---

## 🧪 Test Senaryoları

### ✅ Temel Testler

#### 🎯 Test 1: Ölüm Testi

```bash
./philo 4 310 200 100
```

**Beklenen**: 1. filozof 310ms ±10ms'de ölmeli

#### 🎯 Test 2: Zaman Testi

```bash
./philo 2 800 200 200
```

**Beklenen**: Deadlock olmamalı, filozoflar yemek yemeli

#### 🎯 Test 3: Yemek Sayısı Testi

```bash
./philo 5 800 200 200 7
```

**Beklenen**: Her filozof 7 kez yemek yemeli

### 📊 Test Kriterleri

1. **Zaman hassasiyeti**: ±10ms tolerans
2. **Deadlock yok**: Program sonsuz döngüye girmemeli
3. **Açlık yok**: Tüm filozoflar yemek yeme fırsatı bulmalı
4. **Çatal koruması**: Aynı çatal aynı anda alınamamalı

---

## ❓ Sık Sorulan Sorular

### 🤔 Masa Dönüyor mu?

**Hayır!** Masa dönmüyor. Çatallar **sabit pozisyonlarda** duruyor. Filozoflar **çatalları alıp bırakıyor**.

### 🤔 Neden Çift/Tek ID Farkı?

**Deadlock önleme** için:

- **Çift ID**: Önce sol, sonra sağ çatal
- **Tek ID**: Önce sağ, sonra sol çatal
- Bu sayede **çapraz bekleme** önleniyor

### 🤔 Monitor Thread Ne Yapıyor?

1. **Ölüm kontrolü** yapıyor
2. **Yemek sayısı** takip ediyor
3. **Program sonlandırma** kontrol ediyor

### 🤔 Mutex Sırası Önemli mi?

**Evet!** Mutex'lerin **tutarlı sırada** alınması gerekir:

```c
// DOĞRU: Her zaman aynı sıra
pthread_mutex_lock(&death_mutex);
pthread_mutex_lock(&print_mutex);
// ... işlemler ...
pthread_mutex_unlock(&print_mutex);
pthread_mutex_unlock(&death_mutex);
```

### 🤔 Neden 1 Filozof Özel?

**Çünkü 2 çatal gerekli** ama sadece 1 çatal var. Bu durum **test edilmeli** ve **özel olarak ele alınmalı**.

---

## 🎉 Sonuç

**42 Philosophers** projesi, **multithreading**, **synchronization** ve **resource management** konularını mükemmel şekilde öğreten bir projedir.

### 🏆 Öğrenilen Kavramlar

- ✅ **Thread yönetimi**
- ✅ **Mutex kullanımı**
- ✅ **Deadlock önleme**
- ✅ **Resource allocation**
- ✅ **Time management**
- ✅ **Error handling**

### 🚀 Proje Başarısı

Proje tüm test gereksinimlerini karşılıyor:

- **Zamanlama**: ±10ms tolerans içinde
- **Thread safety**: Data race yok
- **Resource management**: Deadlock yok
- **Performance**: Optimize edilmiş

Bu proje, **gerçek dünya multithreading** problemlerini çözmek için mükemmel bir temel sağlar! 🎯
